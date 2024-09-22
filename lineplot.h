#pragma once

#include "linecurve.h"
#include "linemarker.h"
#include "macroses.h"
#include "selectedmarker.h"
#include "spectrogram.h"

#include <QwtPlot>

#include <any>

class LinePlot : public QwtPlot
{
    Q_OBJECT

public:
    enum Parameters : unsigned int
    {
        xOrigin,    // double
        yOrigin,    // double
        xRange,     // double
        yRange,     // double
        xSamples,   // unsigned int
        ySamples,   // unsigned int
        xAxisName,  // QString
        yAxisName   // QString
    };

    explicit LinePlot(QWidget *parent = nullptr);
    LinePlot(const LinePlot&) = delete;
    LinePlot(LinePlot&&) = delete;
    ~LinePlot() = default;

    LinePlot &operator=(const LinePlot&) = delete;
    LinePlot &operator=(LinePlot&&) = delete;

    void addData(const std::vector<std::vector<double>> &data) UNUSED_FUNCTION
    void addData(const std::vector<double> &data);
    void setData(QVector<double> &&data, const unsigned int nColumns);
    void setParameters(const std::unordered_map<unsigned int, std::any> &parameters);
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
