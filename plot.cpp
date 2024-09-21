#include "plot.h"

#include <QwtInterval>
#include <QwtLinearColorMap>
#include <QwtSymbol>

Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
    , _primarySpectrogram(nullptr)
    , _secondarySpectrogram(nullptr)
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
    this->calculateCurrentY();

    this->_primarySpectrogram->addData(
        data,
        { this->_xAxisOrigin, this->_xAxisEnd },
        { this->_yAxisOrigin, this->_yAxisCurrent }
    );
    this->_lineMarker->setLineCoord(this->_yAxisCurrent);
    this->_lineCurve->setLineData(data, this->_lineCounter);

    this->addCounter();
    this->repaint();

    if (this->isEdgeReached())
    {
        this->swapBuffers();
        this->_forward = ~this->_forward;
    }
}

void Plot::setData(QVector<double> &&data, const unsigned int nColumns)
{
    this->_primarySpectrogram->setData(
        std::move(data),
        nColumns,
        { this->_xAxisOrigin, this->_xAxisEnd },
        { this->_yAxisOrigin, this->_yAxisCurrent }
    );
    this->_lineCurve->hide();
    this->_lineMarker->hide();

    this->replot();
}

void Plot::setParameters(const std::vector<std::any> &parameters)
{
    this->setAxisScale(
        this->xBottom,
        std::any_cast<double>(parameters[0]),
        std::any_cast<double>(parameters[0]) + std::any_cast<double>(parameters[2])
    );
    this->setAxisScale(
        this->yLeft,
        std::any_cast<double>(parameters[1]),
        std::any_cast<double>(parameters[1]) + std::any_cast<double>(parameters[3])
    );
    this->_xAxisEnd = std::any_cast<double>(parameters[0]) + std::any_cast<double>(parameters[2]);
    this->_xAxisOrigin = std::any_cast<double>(parameters[0]);
    this->_xAxisRange = std::any_cast<double>(parameters[2]);
    this->_xSamples = std::any_cast<int>(parameters[4]);
    this->_yAxisEnd = std::any_cast<double>(parameters[1]) + std::any_cast<double>(parameters[3]);
    this->_yAxisOrigin = std::any_cast<double>(parameters[1]);
    this->_yAxisRange = std::any_cast<double>(parameters[3]);
    this->_ySamples = std::any_cast<int>(parameters[5]);
    this->_lineCurve->setParameters(
        std::any_cast<double>(parameters[1]) + std::any_cast<double>(parameters[3]),
        std::any_cast<double>(parameters[1]),
        std::any_cast<int>(parameters[5])
    );
    this->setAxisTitle(this->xBottom, std::any_cast<QString>(parameters[6]));
    this->setAxisTitle(this->yLeft, std::any_cast<QString>(parameters[7]));
    this->replot();
}

void Plot::start()
{
    this->_lineCounter = 1;
    this->_pointCounter = 1;

    this->swapBuffers();
}

void Plot::addCounter()
{
    (this->_forward) ? (++this->_lineCounter) : (--this->_lineCounter);
}

void Plot::calculateCurrentY()
{
    this->_yAxisCurrent = this->_yAxisOrigin + this->_lineCounter * this->_yAxisRange / this->_ySamples;
}

bool Plot::isEdgeReached()
{
    return this->_lineCounter == this->_ySamples + 1 && this->_forward
           || this->_lineCounter == 1 && !this->_forward;
}

void Plot::swapBuffers()
{
    std::swap(this->_primarySpectrogram, this->_secondarySpectrogram);
    if (this->_primarySpectrogram == nullptr)
        this->_primarySpectrogram = std::make_unique<Spectrogram>(this);
    this->_primarySpectrogram->reset();
}
