#include "spectrogram.h"

#include <QVector>
#include <QwtInterval>
#include <QwtPlot>
#include <QwtScaleWidget>

Spectrogram::Spectrogram(QwtPlot *parent)
    : QwtPlotSpectrogram()
    , _rowData(std::make_unique<QVector<double>>())
    , _data(std::make_unique<QwtMatrixRasterData>())
    , _colorMap(std::make_unique<ColorMap>())
    , _parent(parent)
{
    this->setCachePolicy(QwtPlotRasterItem::CachePolicy::PaintCache);
    this->setDisplayMode(QwtPlotSpectrogram::DisplayMode::ImageMode, true);
    this->setColorMap(this->_colorMap.get());
    this->attach(parent);

    _parent->axisWidget(QwtAxis::YRight)->setTitle("z");
    _parent->axisWidget(QwtAxis::YRight)->setColorBarEnabled(true);
    _parent->setAxisVisible(QwtAxis::YRight);

    this->_data->setResampleMode(QwtMatrixRasterData::ResampleMode::BilinearInterpolation);
    this->_data->setAttribute(QwtRasterData::WithoutGaps, true);
    QwtPlotSpectrogram::setData(this->_data.get());
}

void Spectrogram::addData(
    const std::vector<double> &data,
    const std::pair<double, double> &xRange,
    const std::pair<double, double> &yRange
)
{
    this->_rowData->reserve(data.size());
    for (const auto& value : data)
        this->_rowData->push_back(value);

    this->_data->setValueMatrix(*this->_rowData, data.size());
    this->rescale(xRange, yRange);
}

double Spectrogram::maxZ() const
{
    return this->_data->interval(Qt::ZAxis).maxValue();
}

double Spectrogram::minZ() const
{
    return this->_data->interval(Qt::ZAxis).minValue();
}

void Spectrogram::reset()
{
    this->_rowData->clear();
    this->_data.release();
    this->_data = std::make_unique<QwtMatrixRasterData>();
    QwtPlotSpectrogram::setData(this->_data.get());
}

void Spectrogram::setData(
    QVector<double> &&data,
    const unsigned int nColumns,
    const std::pair<double, double> &xRange,
    const std::pair<double, double> &yRange
)
{
    this->_rowData.release();
    this->_rowData = std::make_unique<QVector<double>>(std::move(data));
    this->_data->setValueMatrix(*this->_rowData, nColumns);
    this->rescale(xRange, yRange);
}

QwtInterval Spectrogram::zInterval() const
{
    return this->_data->interval(Qt::ZAxis);
}

void Spectrogram::rescale(
    const std::pair<double, double> &xRange,
    const std::pair<double, double> &yRange
)
{
    this->_data->setInterval(
        Qt::XAxis,
        QwtInterval(xRange.first, xRange.second)
    );
    this->_data->setInterval(
        Qt::YAxis,
        QwtInterval(yRange.first, yRange.second)
    );
    this->_data->setInterval(
        Qt::ZAxis,
        QwtInterval(
            *std::min_element(this->_rowData->begin(), this->_rowData->end()),
            *std::max_element(this->_rowData->begin(), this->_rowData->end())
        )
    );

    _parent->setAxisScale(QwtAxis::YRight, this->minZ(), this->maxZ());
    _parent->axisWidget(QwtAxis::YRight)->setColorMap(this->zInterval(), this->_colorMap.get());
}
