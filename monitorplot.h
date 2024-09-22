#pragma once

#include "macroses.h"
#include "monitorcurve.h"

#include <QwtPlot>

#include <any>

class MonitorPlot : public QwtPlot
{
    Q_OBJECT

public:
    explicit MonitorPlot(QWidget *parent = nullptr);
    MonitorPlot(const MonitorPlot&) = delete;
    MonitorPlot(MonitorPlot&&) = delete;
    ~MonitorPlot() = default;

    MonitorPlot &operator=(const MonitorPlot&) = delete;
    MonitorPlot &operator=(MonitorPlot&&) = delete;

    void addData(const std::vector<std::vector<double>> &data);
    void addData(const std::vector<double> &data);
    void setData(QVector<double> &&data, const unsigned int nColumns) UNUSED_FUNCTION
    void setParameters(const std::unordered_map<unsigned int, std::any> &parameters);
    void start() UNUSED_FUNCTION

private:
    using CurvesVector = std::vector<std::unique_ptr<MonitorCurve>>;
    using ColorsVector = std::vector<QColor>;

    std::unique_ptr<CurvesVector> _curves;
    std::unique_ptr<ColorsVector> _colors;

    double _axisEnd;
    double _axisOrigin;
    double _axisRange;
    unsigned int _samples;

};
