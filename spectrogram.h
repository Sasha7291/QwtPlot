#pragma once

#include "colormap.h"

#include <QwtMatrixRasterData>
#include <QwtPlotSpectrogram>

class Spectrogram : protected QwtPlotSpectrogram
{

public:
    explicit Spectrogram(QwtPlot *parent);
    ~Spectrogram() = default;

    void addData(
        const std::vector<double> &data,
        const std::pair<double, double> &xRange,
        const std::pair<double, double> &yRange
    );
    [[nodiscard]] double maxZ() const;
    [[nodiscard]] double minZ() const;
    void reset();
    void setData(
        QVector<double> &&data,
        const unsigned int nColumns,
        const std::pair<double, double> &xRange,
        const std::pair<double, double> &yRange
    );
    [[nodiscard]] QwtInterval zInterval() const;

private:
    void rescale(
        const std::pair<double, double> &xRange,
        const std::pair<double, double> &yRange
    );

    std::unique_ptr<QVector<double>> _rowData;
    std::unique_ptr<QwtMatrixRasterData> _data;
    std::unique_ptr<ColorMap> _colorMap;
    QwtPlot *_parent;

};
