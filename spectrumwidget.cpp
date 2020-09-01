#include "spectrumwidget.h"
#include <QDebug>

spectrumWidget::spectrumWidget(QWidget *parent) : QWidget(parent)
{
    magnitudes.resize(nbands);
    for (uint i = 0; i < nbands; i++) {
        magnitudes[i] = 0;
    }
}

void spectrumWidget::on_magnitudeschanged(std::vector<float> &mags) {
    magnitudes = mags;

    update();
}

void spectrumWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), height());

    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    float blockwidth = (float)width() / (float)nbands;
    float blockheight;
    QColor color;
    float v;
    for(int i = 0; i < nbands; i++) {
        v = std::min(1.0f, powf(magnitudes[i], 1.25));
        blockheight = height() * v;
        color.setHsv(i*360/nbands, 255 * v, 255);
        painter.setBrush(color);
        painter.drawRect(
            i*blockwidth,
            height()-blockheight,
            blockwidth,
            blockheight + 1
        );
    }
}
