#include "gstspectrum.h"
#include <QDebug>

void GstSpectrum::on_message_element(const GstBus *bus, GstMessage *message, GstSpectrum *self) {
    if (GST_OBJECT_NAME(message->src) == std::string("spectrum")) {
        const GstStructure *s = gst_message_get_structure(message);

        const GValue *magnitudes = gst_structure_get_value(s, "magnitude");

        for (uint n = 0; n < self->spectrum_freqs.size(); n++) {
            self->spectrum_mag_tmp[n] = g_value_get_float(gst_value_list_get_value(magnitudes, n + self->spline_f0));
        }

        boost::math::interpolators::cardinal_cubic_b_spline<float> spline(
            self->spectrum_mag_tmp.begin(), self->spectrum_mag_tmp.end(), self->spline_f0, self->spline_df
        );

        for (uint n = 0; n < self->spectrum_mag.size(); n++) {
            self->spectrum_mag[n] = spline(self->spectrum_x_axis[n]);
        }

        float min_mag = self->spectrum_threshold;
        float max_mag = *std::max_element(self->spectrum_mag.begin(), self->spectrum_mag.end());

        if (max_mag > min_mag) {
            for (float &v : self->spectrum_mag) {
                if (v > min_mag)
                    v = (min_mag - v) / min_mag;
                else
                    v = 0.0;
            }
        } else {
            for (float &v : self->spectrum_mag) {
                v = 0.0f;
            }
        }
        emit self->on_new_spectrum(self->spectrum_mag);
    }
}

GstSpectrum::GstSpectrum() {
    gst_init(nullptr, nullptr);

    pipeline = gst_pipeline_new("pipeline");

    bus = gst_element_get_bus(pipeline);

    gst_bus_add_signal_watch(bus);

    // bus callback

    g_signal_connect(bus, "message::element", G_CALLBACK(on_message_element), this);

    // creating elements

    source = gst_element_factory_make("pulsesrc", "source");
    sink = gst_element_factory_make("fakesink", "sink");
    spectrum = gst_element_factory_make("spectrum", "spectrum");

    capsfilter = gst_element_factory_make("capsfilter", nullptr);
    queue = gst_element_factory_make("queue", nullptr);

    // building the pipeline

    gst_bin_add_many(GST_BIN(pipeline), source, capsfilter, queue, spectrum, sink, nullptr);
    gst_element_link_many(source, capsfilter, queue, spectrum, sink, nullptr);

    // setting a few parameters

    auto props = gst_structure_from_string("props,application.name=PulseEffectsCalibration", nullptr);
    auto caps = gst_caps_from_string("audio/x-raw,format=F32LE,channels=2,rate=48000");

    // desktop speakers
    // g_object_set(source, "device", "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor", nullptr);
    // desktop headset
    g_object_set(source, "device", "alsa_output.usb-Kingston_HyperX_Virtual_Surround_Sound_00000000-00.analog-stereo.monitor", nullptr);
    // laptop speakers
    // g_object_set(source, "device", "alsa_output.pci-0000_00_1f.3.analog-stereo.monitor", nullptr);
    g_object_set(capsfilter, "caps", caps, nullptr);
    g_object_set(queue, "silent", true, nullptr);
    g_object_set(spectrum, "bands", spectrum_nbands, nullptr);
    g_object_set(spectrum, "threshold", spectrum_threshold, nullptr);

    gst_structure_free(props);
    gst_caps_unref(caps);

    // init spectrum

    for (uint n = 0; n < spectrum_nbands; n++) {
        auto f = 48000 * (0.5 * n + 0.25) / spectrum_nbands;
        if (f > max_spectrum_freq)
            break;
        if (f > min_spectrum_freq)
            spectrum_freqs.emplace_back(f);
    }

    spectrum_mag_tmp.resize(spectrum_freqs.size());

    spectrum_x_axis = util::logspace(log10(min_spectrum_freq), log10(max_spectrum_freq), spectrum_npoints);

    spectrum_mag.resize(spectrum_npoints);

    spline_f0 = spectrum_freqs[0];
    spline_df = spectrum_freqs[1] - spectrum_freqs[0];
}

GstSpectrum::~GstSpectrum() {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(bus);
    gst_object_unref(pipeline);
}

void GstSpectrum::run() {
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(pipeline);
}

void GstSpectrum::play() {
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void GstSpectrum::pause() {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
}

void GstSpectrum::exit() {
    g_main_loop_quit(loop);
}
