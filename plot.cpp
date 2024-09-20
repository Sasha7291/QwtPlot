#include "plot.h"

#include <QwtInterval>
#include <QwtLinearColorMap>
#include <QwtSymbol>

Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
    , _spectrogram(std::make_unique<Spectrogram>(this))
    , _curve(std::make_unique<QwtPlotCurve>())
    , _selectedMarker(std::make_unique<SelectedMarker>(this))
    , _lineMarker(std::make_unique<QwtPlotMarker>())
    , _lineCounter(1)
    , _pointCounter(1)
    , _xAxisCurrent(0.0)
    , _xAxisEnd(100.0)
    , _xAxisOrigin(0.0)
    , _xAxisRange(100.0)
    , _xSamples(100)
    , _yAxisCurrent(0.0)
    , _yAxisEnd(100.0)
    , _yAxisOrigin(0.0)
    , _yAxisRange(100.0)
    , _ySamples(100)
{
    this->setAxisAutoScale(Qt::YAxis, true);
    this->setAutoDelete(false);

    this->_curve->setPen(QPen(Qt::red, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->_curve->setStyle(QwtPlotCurve::Lines);
    this->_curve->attach(this);

    this->_lineMarker->setLineStyle(QwtPlotMarker::HLine);
    this->_lineMarker->setLinePen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->_lineMarker->setValue(0.0, 0.0);
    this->_lineMarker->attach(this);

    QwtLinearColorMap *colorMap = new QwtLinearColorMap(Qt::black, Qt::white);
    colorMap->setMode(QwtLinearColorMap::ScaledColors);
    colorMap->addColorStop(0.1, qRgb(20, 7, 6));
    colorMap->addColorStop(0.2, qRgb(42, 27, 6));
    colorMap->addColorStop(0.3, qRgb(100, 47, 6));
    colorMap->addColorStop(0.4, qRgb(179, 84, 17));
    colorMap->addColorStop(0.5, qRgb(204, 118, 30));
    colorMap->addColorStop(0.6, qRgb(230, 159, 43));
    colorMap->addColorStop(0.7, qRgb(255, 200, 57));
    colorMap->addColorStop(0.8, qRgb(255, 234, 100));
    colorMap->addColorStop(0.9, qRgb(255, 255, 138));
    this->_spectrogram->setColorMap(colorMap);
}

void Plot::addData(const std::vector<double> &data)
{
    this->_yAxisCurrent = this->_yAxisOrigin + this->_lineCounter * this->_yAxisRange / this->_ySamples;

    this->_spectrogram->addData(
        data,
        { this->_xAxisOrigin, this->_xAxisEnd },
        { this->_yAxisOrigin, this->_yAxisCurrent }
    );
    this->_lineMarker->setYValue(this->_yAxisCurrent);

    std::vector<double> values;
    values.reserve(data.size());
    this->_curve->setSamples(values.data(), values.size());

    ++this->_lineCounter;
    this->repaint();
}

void Plot::setParameters(
    const double xOrigin,
    const double yOrigin,
    const double xRange,
    const double yRange,
    const unsigned int xSamples,
    const unsigned int ySamples,
    const QString &xAxisName,
    const QString &yAxisName
)
{
    this->setAxisScale(this->xBottom, xOrigin, xOrigin + xRange);
    this->setAxisScale(this->yLeft, yOrigin, yOrigin + yRange);
    this->_xAxisEnd = xOrigin + xRange;
    this->_xAxisOrigin = xOrigin;
    this->_xAxisRange = xRange;
    this->_xSamples = xSamples;
    this->_yAxisEnd = yOrigin + yRange;
    this->_yAxisOrigin = yOrigin;
    this->_yAxisRange = yRange;
    this->_ySamples = ySamples;
    this->_lineMarker->setYValue(this->_yAxisOrigin);
    this->setAxisTitle(this->xBottom, xAxisName);
    this->setAxisTitle(this->yLeft, yAxisName);
    this->repaint();
}
