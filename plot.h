#pragma once

#include "linecurve.h"
#include "linemarker.h"
#include "selectedmarker.h"
#include "spectrogram.h"

#include <QwtPlot>

#include <any>

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
    void setData(QVector<double> &&data, const unsigned int nColumns);
    /* Последовательность параметров:
     * 0. xOrigin
     * 1. yOrigin
     * 2. xRange
     * 3. yRange
     * 4. xSamples
     * 5. ySamples
     * 6. xAxisName
     * 7. yAxisName
     */
    void setParameters(const std::vector<std::any> &parameters);
    void start();

private:
    void addCounter();
    void calculateCurrentY();
    bool isEdgeReached();
    void swapBuffers();

    std::unique_ptr<Spectrogram> _primarySpectrogram;
    std::unique_ptr<Spectrogram> _secondarySpectrogram;
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
    bool _forward;

};
