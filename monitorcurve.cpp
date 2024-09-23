#include "monitorcurve.h"

#include <QDebug>
#include <QPen>

MonitorCurve::MonitorCurve(QwtPlot *parent, const QColor color, const QString name)
    : QwtPlotCurve()
    , _xValues(std::make_unique<QVector<double>>())
    , _parent(parent)
{
    this->setPen(QPen(color, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    this->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    this->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    this->setTitle(name);
    this->attach(parent);
}

void MonitorCurve::setData(const std::vector<double> &data)
{
    this->setSamples(this->_xValues->constData(), data.data(), data.size());
}

void MonitorCurve::setXValues(const double origin, const double range, const unsigned int nSamples)
{
    this->_xValues->clear();
    this->_xValues->reserve(nSamples);
    for (unsigned int i = 0; i < nSamples; ++i)
        this->_xValues->push_back(i / static_cast<double>(nSamples) * range + origin);
}
