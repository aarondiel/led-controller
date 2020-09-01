#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <algorithm>
#include "gstspectrum.h"

class spectrumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit spectrumWidget(QWidget *parent = nullptr);
public slots:
    void on_magnitudeschanged(std::vector<float> &mags);
private:
    std::vector<float> magnitudes;
    void paintEvent(QPaintEvent *event);
};

#endif // SPECTRUMWIDGET_H
