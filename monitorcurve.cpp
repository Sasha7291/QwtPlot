#include "monitorcurve.h"

#include <QPen>

MonitorCurve::MonitorCurve(QwtPlot *parent, QColor color)
    : QwtPlotCurve()
    , _parent(parent)
{
    this->setPen(QPen(color, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->attach(parent);
}

void MonitorCurve::setData(const std::vector<double> &data)
{
    this->setSamples(data.data(), data.size());
}
