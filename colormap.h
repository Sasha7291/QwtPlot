#pragma once

#include <QwtColorMap>

class ColorMap : public QwtLinearColorMap
{

public:
    ColorMap()
        : QwtLinearColorMap(Qt::black, Qt::white)
    {
        this->setMode(QwtLinearColorMap::ScaledColors);
        this->addColorStop(0.1, qRgb(20, 7, 6));
        this->addColorStop(0.2, qRgb(42, 27, 6));
        this->addColorStop(0.3, qRgb(100, 47, 6));
        this->addColorStop(0.4, qRgb(179, 84, 17));
        this->addColorStop(0.5, qRgb(204, 118, 30));
        this->addColorStop(0.6, qRgb(230, 159, 43));
        this->addColorStop(0.7, qRgb(255, 200, 57));
        this->addColorStop(0.8, qRgb(255, 234, 100));
        this->addColorStop(0.9, qRgb(255, 255, 138));
    }

};
