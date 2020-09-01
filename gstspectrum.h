#ifndef GSTSPECTRUM_H
#define GSTSPECTRUM_H

#include <QObject>
#include <gst/gst.h>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include "util.h"

const int nbands = 100;
const int audiofreq = 32000;
const int audiothreshold = -120;

class GstSpectrum: public QObject
{
    Q_OBJECT
public:
    GstSpectrum();
    ~GstSpectrum();

public slots:
    void run();
    void pause();
    void play();
    void exit();

private:
    static void on_message_element(const GstBus *bus, GstMessage *message, GstSpectrum *self);
    GstElement
        *pipeline = nullptr,
        *source= nullptr,
        *capsfilter = nullptr,
        *queue = nullptr,
        *spectrum = nullptr,
        *sink = nullptr;
    GMainLoop *loop = nullptr;
    GstBus *bus = nullptr;
    uint min_spectrum_freq = 20;
    uint max_spectrum_freq = 20000;
    int spectrum_threshold = -120;
    uint spectrum_nbands = 3200;
    uint spectrum_npoints = 100;
    float spline_f0, spline_df;
    int average = 0;
    std::vector<float> spectrum_freqs, spectrum_x_axis;
    std::vector<float> spectrum_mag_tmp, spectrum_mag;

signals:
    void on_new_spectrum(std::vector<float> &magnitudes);
};

#endif // GSTSPECTRUM_H
