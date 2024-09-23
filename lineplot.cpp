#include "lineplot.h"

#include <QwtInterval>
#include <QwtLinearColorMap>
#include <QwtPlotLayout>
#include <QwtSymbol>

LinePlot::LinePlot(QWidget *parent)
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
    , _forward(true)
{
    this->plotLayout()->setAlignCanvasToScales(true);
    this->setAxisAutoScale(Qt::YAxis, true);
    this->setAutoDelete(false);
    this->setAxisFont(this->xBottom, QFont("Bahnschrift"));
    this->setAxisFont(this->yLeft, QFont("Bahnschrift"));
    this->setAxisFont(Qt::YAxis, QFont("Bahnschrift"));
}

void LinePlot::addData(const std::vector<double> &data)
{
    this->calculateCurrentY();

    this->_primarySpectrogram->addData(
        data,
        { this->_xAxisOrigin, this->_xAxisEnd },
        {
            this->_forward ? this->_yAxisOrigin : this->_yAxisCurrent,
            this->_forward ? this->_yAxisCurrent : this->_yAxisEnd
        }
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

void LinePlot::setData(QVector<double> &&data, const unsigned int nColumns)
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

void LinePlot::setParameters(const std::unordered_map<unsigned int, std::any> &parameters)
{
    this->setAxisScale(
        this->xBottom,
        std::any_cast<double>(parameters.at(xOrigin)),
        std::any_cast<double>(parameters.at(xOrigin)) + std::any_cast<double>(parameters.at(xRange))
    );
    this->setAxisScale(
        this->yLeft,
        std::any_cast<double>(parameters.at(yOrigin)),
        std::any_cast<double>(parameters.at(yOrigin)) + std::any_cast<double>(parameters.at(yRange))
    );
    this->_xAxisEnd = std::any_cast<double>(parameters.at(xOrigin)) + std::any_cast<double>(parameters.at(xRange));
    this->_xAxisOrigin = std::any_cast<double>(parameters.at(xOrigin));
    this->_xAxisRange = std::any_cast<double>(parameters.at(xRange));
    this->_xSamples = std::any_cast<unsigned int>(parameters.at(xSamples));
    this->_yAxisEnd = std::any_cast<double>(parameters.at(yOrigin)) + std::any_cast<double>(parameters.at(yRange));
    this->_yAxisOrigin = std::any_cast<double>(parameters.at(yOrigin));
    this->_yAxisRange = std::any_cast<double>(parameters.at(yRange));
    this->_ySamples = std::any_cast<unsigned int>(parameters.at(ySamples));
    this->_lineCurve->setParameters(
        std::any_cast<double>(parameters.at(yOrigin)) + std::any_cast<double>(parameters.at(yRange)),
        std::any_cast<double>(parameters.at(yOrigin)),
        std::any_cast<unsigned int>(parameters.at(ySamples))
    );
    this->setAxisTitle(this->xBottom, std::any_cast<QString>(parameters.at(xAxisName)));
    this->setAxisTitle(this->yLeft, std::any_cast<QString>(parameters.at(yAxisName)));
    this->replot();
}

void LinePlot::start()
{
    this->_lineCounter = 1;
    this->_pointCounter = 1;
    this->_forward = true;

    this->swapBuffers();
}

void LinePlot::addCounter()
{
    (this->_forward) ? (++this->_lineCounter) : (--this->_lineCounter);
}

void LinePlot::calculateCurrentY()
{
    if (this->_forward)
        this->_yAxisCurrent = this->_yAxisOrigin + this->_lineCounter * this->_yAxisRange / this->_ySamples;
    else
        this->_yAxisCurrent = this->_yAxisEnd - this->_lineCounter * this->_yAxisRange / this->_ySamples;
}

bool LinePlot::isEdgeReached()
{
    return this->_lineCounter == this->_ySamples + 1 && this->_forward
           || this->_lineCounter == 1 && !this->_forward;
}

void LinePlot::swapBuffers()
{
    std::swap(this->_primarySpectrogram, this->_secondarySpectrogram);
    if (this->_primarySpectrogram == nullptr)
        this->_primarySpectrogram = std::make_unique<Spectrogram>(this);
    this->_primarySpectrogram->reset();
}
