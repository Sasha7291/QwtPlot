#include "monitorplot.h"

#include <QwtLegend>
#include <QwtPlotCanvas>
#include <QwtPlotLayout>

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
    , _grid(std::make_unique<Grid>(this))
    , _legend(std::make_unique<Legend>(this))
    , _movedMarker(std::make_unique<MovedMarker>(this))
    , _curveNames({
        "Curve1",
        "Curve2",
        "Curve3",
        "Curve4",
        "Curve5",
        "Curve6",
        "Curve7",
        "Curve8",
        "Curve9",
        "Curve10",
        "Curve11",
        "Curve12"
    })
{
    std::generate(this->_curves->begin(), this->_curves->end(), [](){ return nullptr; });
    dynamic_cast<QwtPlotCanvas*>(this->canvas())->setPaintAttribute(QwtPlotCanvas::BackingStore, false);
    dynamic_cast<QwtPlotCanvas*>(this->canvas())->setBorderRadius(10);
    this->plotLayout()->setAlignCanvasToScales(true);
    this->setAxisAutoScale(this->yLeft, true);
    this->setCanvasBackground(QBrush(QColor(38, 64, 115)));
    this->setAxisFont(this->xBottom, QFont("Bahnschrift"));
    this->setAxisFont(this->yLeft, QFont("Bahnschrift"));

    connect(this->_movedMarker.get(), &MovedMarker::deltaCalculated, this, &MonitorPlot::deltaCalculated);
}

void MonitorPlot::addData(const std::vector<std::vector<double>> &data)
{
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        if (this->_curves->at(i) == nullptr)
        {
            this->_curves->at(i) = std::make_unique<MonitorCurve>(
                this,
                this->_colors->at(i),
                this->_curveNames.at(i)
            );
            this->_curves->at(i)->setXValues(this->_origin, this->_range, data.size());
        }

        this->_curves->at(i)->setData(data[i]);
    }
}

void MonitorPlot::addData(const std::vector<double> &data)
{
    if (this->_curves->at(0) == nullptr)
    {
        this->_curves->at(0) = std::make_unique<MonitorCurve>(
            this,
            this->_colors->at(0),
            this->_curveNames.at(0)
        );
        this->_curves->at(0)->setXValues(this->_origin, this->_range, data.size());
    }

    this->_curves->at(0)->setData(data);
}

void MonitorPlot::setParameters(const std::unordered_map<unsigned int, std::any> &parameters)
{
    this->_origin = std::any_cast<double>(parameters.at(origin));
    this->_range = std::any_cast<double>(parameters.at(end)) - std::any_cast<double>(parameters.at(origin));
    this->setAxisScale(
        this->xBottom,
        std::any_cast<double>(parameters.at(origin)),
        std::any_cast<double>(parameters.at(end))
    );
    this->setAxisTitle(this->xBottom, std::any_cast<QString>(parameters.at(xAxisName)));
    this->setAxisTitle(this->yLeft, std::any_cast<QString>(parameters.at(yAxisName)));
    this->_curveNames = std::any_cast<QStringList>(parameters.at(curveNames));
    this->_movedMarker->reset(
        std::any_cast<double>(parameters.at(origin)),
        std::any_cast<double>(parameters.at(end))
    );
    for (auto &curve : *this->_curves)
        if (curve != nullptr)
            curve->setXValues(this->_origin, this->_range, std::any_cast<unsigned int>(parameters.at(nSamples)));
    this->replot();
}
