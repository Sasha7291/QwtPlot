#pragma once

#include <QwtMatrixRasterData>
#include <QwtPlotSpectrogram>

class Spectrogram : public QwtPlotSpectrogram
{

public:
    explicit Spectrogram(QwtPlot *parent);
    ~Spectrogram() = default;

    void addData(
        const std::vector<double> &data,
        const std::pair<double, double> &xRange,
        const std::pair<double, double> &yRange
    );
    void reset();

private:
    std::unique_ptr<QVector<double>> _rowData;
    std::unique_ptr<QwtMatrixRasterData> _data;

};
