#include "plot.h"

#include <QwtInterval>
#include <QwtLinearColorMap>
#include <QwtSymbol>

Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
    , _spectrogram(std::make_unique<Spectrogram>(this))
    , _lineCurve(std::make_unique<LineCurve>(this))
    , _selectedMarker(std::make_unique<SelectedMarker>(this))
    , _lineMarker(std::make_unique<LineMarker>(this))
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
}

void Plot::addData(const std::vector<double> &data)
{
    this->_yAxisCurrent = this->_yAxisOrigin + this->_lineCounter * this->_yAxisRange / this->_ySamples;

    this->_spectrogram->addData(
        data,
        { this->_xAxisOrigin, this->_xAxisEnd },
        { this->_yAxisOrigin, this->_yAxisCurrent }
    );
    this->_lineMarker->setLineCoord(this->_yAxisCurrent);
    this->_lineCurve->setLineData(data, this->_lineCounter);

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
    this->_lineCurve->setParameters(yOrigin + yRange, yOrigin, ySamples);
    this->setAxisTitle(this->xBottom, xAxisName);
    this->setAxisTitle(this->yLeft, yAxisName);
    this->repaint();
}
