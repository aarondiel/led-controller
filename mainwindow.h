#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <algorithm>
#include "ui_mainwindow.h"
#include "util.h"
#include "gstspectrum.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_new_spectrum(std::vector<float> &magnitudes);

    void on_onButton_toggled(bool checked);

    void on_offButton_toggled(bool checked);

    void on_hueSlider_valueChanged();

    void on_saturationSlider_valueChanged();

    void on_valueSlider_valueChanged();

    void on_redSlider_valueChanged();

    void on_greenSlider_valueChanged();

    void on_blueSlider_valueChanged();

    void on_startConnection_clicked();

    void solid_color_serial();

    void cycle_hue_serial();

    void spectrum_serial();

    void no_color_serial();

    void on_colorfunctionComboBox_currentTextChanged(const QString &txt);

private:
    GstSpectrum spectrumanalyzer;
    QThread spectrumthread;
    Ui::MainWindow *ui;
    QSerialPort serial_connection;
    QTimer *serialTimer;
    float magnitude_freq;
    float magnitude_volume;
    void update_rgb();
    void update_hsv();
};
#endif // MAINWINDOW_H
