#include "monitorplot.h"

MonitorPlot::MonitorPlot(QWidget *parent)
    : QwtPlot(parent)
    , _curves(std::make_unique<CurvesVector>(12))
    , _colors(std::make_unique<ColorsVector>(ColorsVector(
    {
        Qt::red,
        Qt::green,
        Qt::cyan,
        Qt::magenta,
        Qt::yellow,
        Qt::gray,
        Qt::darkRed,
        Qt::darkGreen,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow,
        Qt::darkGray
    })))
    , _axisEnd(100.0)
    , _axisOrigin(0.0)
    , _axisRange(100.0)
    , _samples(100)
{
    std::generate(this->_curves->begin(), this->_curves->end(), [](){ return nullptr; });
    this->setAxisAutoScale(this->yLeft, true);
    this->setCanvasBackground(QBrush(Qt::darkBlue));
}

void MonitorPlot::addData(const std::vector<std::vector<double>> &data)
{
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        if (this->_curves->at(i) == nullptr)
            this->_curves->at(i) = std::make_unique<MonitorCurve>(this, this->_colors->at(i));

        this->_curves->at(i)->setData(data[i]);
    }
}

void MonitorPlot::addData(const std::vector<double> &data)
{
    if (this->_curves->at(0) == nullptr)
        this->_curves->at(0) = std::make_unique<MonitorCurve>(this, this->_colors->at(0));

    this->_curves->at(0)->setData(data);
}

void MonitorPlot::setParameters(const std::unordered_map<unsigned int, std::any> &parameters)
{

}

