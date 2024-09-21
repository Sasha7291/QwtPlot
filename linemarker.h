#pragma once

#include <QwtPlotMarker>

class LineMarker : private QwtPlotMarker
{

public:
    explicit LineMarker(QwtPlot *parent);
    ~LineMarker() = default;

    [[nodiscard]] double lineCoord() const { return this->yValue(); }
    inline void setLineCoord(const double coord) { this->setYValue(coord); }

};
