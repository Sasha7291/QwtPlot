#include "linecurve.h"

#include <QPen>

LineCurve::LineCurve(QwtPlot *parent)
    : QwtPlotCurve()
    , _parent(parent)
    , _max(100.0)
    , _min(0.0)
    , _samples(100)
{
    this->setPen(QPen(Qt::red, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    this->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    this->setStyle(QwtPlotCurve::Lines);
    this->attach(parent);
}

void LineCurve::setLineData(const std::vector<double> &data, const unsigned int lineNumber)
{
    std::vector<double> values(data);
    std::transform(values.begin(), values.end(), values.begin(), [this, lineNumber](double value){
        return value * 10.0 / (this->_max - this->_min) - 10.0 + lineNumber * (this->_max - this->_min) / this->_samples;
    });

    auto max = *std::max_element(values.begin(), values.end());
    auto min = *std::min_element(values.begin(), values.end());
    if (max > this->_max)
        std::transform(values.begin(), values.end(), values.begin(), [this, max](double value){
            return value - (max - this->_max);
        });
    else if (min < this->_min)
        std::transform(values.begin(), values.end(), values.begin(), [this, min](double value){
            return value + (this->_min - min);
        });

    this->setSamples(values.data(), values.size());
}
