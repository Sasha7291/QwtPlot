#include "spectrogram.h"

#include <QVector>
#include <QwtInterval>

Spectrogram::Spectrogram(QwtPlot *parent)
    : QwtPlotSpectrogram()
    , _rowData(std::make_unique<QVector<double>>())
    , _data(std::make_unique<QwtMatrixRasterData>())
{
    this->setCachePolicy(QwtPlotRasterItem::CachePolicy::PaintCache);
    this->setDisplayMode(QwtPlotSpectrogram::DisplayMode::ContourMode, true);
    this->attach(parent);

    this->_data->setResampleMode(QwtMatrixRasterData::ResampleMode::BilinearInterpolation);
    this->_data->setAttribute(QwtRasterData::WithoutGaps, true);

    this->setData(this->_data.get());
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
}

void Spectrogram::reset()
{
    this->_rowData->clear();
    this->_data.release();
    this->_data = std::make_unique<QwtMatrixRasterData>();
    this->setData(this->_data.get());
}
