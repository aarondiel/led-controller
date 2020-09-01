#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSerialPort serial_connection;

    serialTimer = new QTimer(this);
    connect(serialTimer, SIGNAL(timeout()), this, SLOT(solid_color_serial()));

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        ui->portComboBox->addItem(info.portName());
    }

    spectrumanalyzer.moveToThread(&spectrumthread);
    connect(
        &spectrumanalyzer, SIGNAL(on_new_spectrum(std::vector<float>&)),
        ui->spectrum, SLOT(on_magnitudeschanged(std::vector<float>&))
    );
    connect(&spectrumthread, SIGNAL(started()), &spectrumanalyzer, SLOT(run()));
    spectrumthread.start();

    connect(
        &spectrumanalyzer, SIGNAL(on_new_spectrum(std::vector<float>&)),
        this, SLOT(on_new_spectrum(std::vector<float>&))
    );
}

MainWindow::~MainWindow() {
    if(serial_connection.isOpen())
        serial_connection.close();

    spectrumanalyzer.exit();
    this->thread()->msleep(1000);
    spectrumthread.quit();

    delete ui;
    delete serialTimer;
}

void MainWindow::on_new_spectrum(std::vector<float> &magnitudes) {
    this->magnitude_freq = 0.0;
    float weights = 0.0;
    for (uint i = 0; i < magnitudes.size(); i++) {
        this->magnitude_freq += magnitudes[i] * i;
        weights += magnitudes[i];
    }
    if (weights != 0.0) {
        this->magnitude_freq /= weights;
        this->magnitude_freq /= magnitudes.size();
        this->magnitude_volume = *std::max_element(magnitudes.begin(), magnitudes.end());
    } else {
        this->magnitude_freq = 0;
        this->magnitude_volume = 0;
    }
}

void MainWindow::on_onButton_toggled(bool checked) {
    ui->offButton->blockSignals(true);

    if(checked)
        ui->offButton->setChecked(false);
    else
        ui->onButton->setChecked(true);

    ui->offButton->blockSignals(false);

    if(serial_connection.isOpen())
        serialTimer->start(10);
}

void MainWindow::on_offButton_toggled(bool checked) {
    ui->onButton->blockSignals(true);

    if(checked)
        ui->onButton->setChecked(false);
    else
        ui->offButton->setChecked(true);

    ui->onButton->blockSignals(false);

    if(serial_connection.isOpen())
        serialTimer->stop();
}

void MainWindow::update_rgb() {
    ui->redSlider->blockSignals(true);
    ui->greenSlider->blockSignals(true);
    ui->blueSlider->blockSignals(true);

    int r = 0;
    int g = 0;
    int b = 0;

    util::hsvtorgb(
        ui->hueSlider->value(),
        ui->saturationSlider->value(),
        ui->valueSlider->value(),
        &r, &g, &b
    );

    ui->redSlider->setValue(r);
    ui->greenSlider->setValue(g);
    ui->blueSlider->setValue(b);

    ui->redSlider->blockSignals(false);
    ui->greenSlider->blockSignals(false);
    ui->blueSlider->blockSignals(false);
}

void MainWindow::update_hsv() {
    ui->hueSlider->blockSignals(true);
    ui->saturationSlider->blockSignals(true);
    ui->valueSlider->blockSignals(true);

    int h = 0;
    int s = 0;
    int v = 0;

    util::rgbtohsv(
        ui->redSlider->value(),
        ui->greenSlider->value(),
        ui->blueSlider->value(),
        &h, &s, &v
    );

    ui->hueSlider->setValue(h);
    ui->saturationSlider->setValue(s);
    ui->valueSlider->setValue(v);

    ui->hueSlider->blockSignals(false);
    ui->saturationSlider->blockSignals(false);
    ui->valueSlider->blockSignals(false);
}

void MainWindow::on_hueSlider_valueChanged() {
    update_rgb();
}

void MainWindow::on_saturationSlider_valueChanged() {
    update_rgb();
}

void MainWindow::on_valueSlider_valueChanged() {
    update_rgb();
}

void MainWindow::on_redSlider_valueChanged() {
    update_hsv();
}

void MainWindow::on_greenSlider_valueChanged() {
    update_hsv();
}

void MainWindow::on_blueSlider_valueChanged() {
    update_hsv();
}

void MainWindow::on_startConnection_clicked() {
    serial_connection.setPortName(ui->portComboBox->currentText());

    if(!serial_connection.open(QIODevice::ReadWrite)) {
        return;
    }

    if(
        !serial_connection.setBaudRate(9600) |
        !serial_connection.setDataBits(QSerialPort::Data8) |
        !serial_connection.setParity(QSerialPort::NoParity) |
        !serial_connection.setStopBits(QSerialPort::OneStop) |
        !serial_connection.setFlowControl(QSerialPort::NoFlowControl)
    ) {
        return;
    }

    ui->startConnection->setEnabled(false);
}

void MainWindow::solid_color_serial() {
    if(serial_connection.readAll() != "") {
        char data[3] = {
            (char)ui->redSlider->value(),
            (char)ui->greenSlider->value(),
            (char)ui->blueSlider->value()
        };

        serial_connection.write(data, 3 * sizeof(char));
    }
}

void MainWindow::cycle_hue_serial() {
    ui->hueSlider->setValue((ui->hueSlider->value() + 1) % 360);
    solid_color_serial();
}

void MainWindow::spectrum_serial() {
    ui->hueSlider->setValue(fmod(4 * this->magnitude_freq * 360.0, 360.0));
    int brightness = 100 * this->magnitude_volume / 2;

    ui->valueSlider->setValue(brightness);
    update_rgb();
    solid_color_serial();
}

void MainWindow::no_color_serial() {
    if(serial_connection.readAll() != "") {
        char data[3] = {0, 0, 0};
        serial_connection.write(data, 6 * sizeof(char));
    }
}

void MainWindow::on_colorfunctionComboBox_currentTextChanged(const QString &txt) {
    serialTimer->disconnect();

    if (txt == "solid color") {
        connect(serialTimer, SIGNAL(timeout()), this, SLOT(solid_color_serial()));
    } else if (txt == "cycle hue") {
        connect(serialTimer, SIGNAL(timeout()), this, SLOT(cycle_hue_serial()));
    } else if (txt == "spectrum") {
        connect(serialTimer, SIGNAL(timeout()), this, SLOT(spectrum_serial()));
    } else if (txt == "no color") {
        connect(serialTimer, SIGNAL(timeout()), this, SLOT(no_color_serial()));
    }
}
