#pragma once

#include "linecurve.h"
#include "linemarker.h"
#include "selectedmarker.h"
#include "spectrogram.h"

#include <QwtPlot>

class SelectedMarker;
class Spectrogram;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = nullptr);
    Plot(const Plot&) = delete;
    Plot(Plot&&) = delete;
    ~Plot() = default;

    Plot &operator=(const Plot&) = delete;
    Plot &operator=(Plot&&) = delete;

    void addData(const std::vector<double> &data);
    void setParameters(
        const double xOrigin,
        const double yOrigin,
        const double xRange,
        const double yRange,
        const unsigned int xSamples,
        const unsigned int ySamples,
        const QString &xAxisName,
        const QString &yAxisName
    );

private:
    std::unique_ptr<Spectrogram> _spectrogram;
    std::unique_ptr<LineCurve> _lineCurve;
    std::unique_ptr<SelectedMarker> _selectedMarker;
    std::unique_ptr<LineMarker> _lineMarker;

    unsigned int _pointCounter;
    unsigned int _lineCounter;
    double _xAxisCurrent;
    double _xAxisEnd;
    double _xAxisOrigin;
    double _xAxisRange;
    unsigned int _xSamples;
    double _yAxisCurrent;
    double _yAxisEnd;
    double _yAxisOrigin;
    double _yAxisRange;
    unsigned int _ySamples;

};
