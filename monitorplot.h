#pragma once

#include "grid.h"
#include "legend.h"
#include "macroses.h"
#include "monitorcurve.h"
#include "movedmarker.h"

#include <QwtPlot>

#include <any>

class MonitorPlot : public QwtPlot
{
    Q_OBJECT

public:
    enum Parameters : unsigned int
    {
        origin,     // double
        end,        // double
        nSamples,   // unsigned int
        xAxisName,  // QString
        yAxisName,  // QString
        curveNames  // QStringList
    };

    explicit MonitorPlot(QWidget *parent = nullptr);
    MonitorPlot(const MonitorPlot&) = delete;
    MonitorPlot(MonitorPlot&&) = delete;
    ~MonitorPlot() = default;

    MonitorPlot &operator=(const MonitorPlot&) = delete;
    MonitorPlot &operator=(MonitorPlot&&) = delete;

    void addData(const std::vector<std::vector<double>> &data);
    void addData(const std::vector<double> &data);
    QwtPlotCurve *curve() const { return this->_curves->at(0).get(); }
    void setData(QVector<double> &&data, const unsigned int nColumns) UNUSED_FUNCTION
    void setParameters(const std::unordered_map<unsigned int, std::any> &parameters);
    void start() UNUSED_FUNCTION

signals:
    void deltaCalculated(const double xDelta, const double yDelta);

private:
    using CurvesVector = std::vector<std::unique_ptr<MonitorCurve>>;
    using ColorsVector = std::vector<QColor>;

    std::unique_ptr<CurvesVector> _curves;
    std::unique_ptr<ColorsVector> _colors;
    std::unique_ptr<Grid> _grid;
    std::unique_ptr<Legend> _legend;
    std::unique_ptr<MovedMarker> _movedMarker;
    QStringList _curveNames;

    double _origin;
    double _range;

};
