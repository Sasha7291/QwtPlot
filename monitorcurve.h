#pragma once

#include <QColor>
#include <QwtPlotCurve>

class MonitorCurve : public QwtPlotCurve
{

public:
    explicit MonitorCurve(QwtPlot *parent, const QColor color = Qt::red, const QString name = "Curve");

    void setData(const std::vector<double> &data);
    void setXValues(
        const double origin,
        const double range,
        const unsigned int nSamples
    );

private:
    std::unique_ptr<QVector<double>> _xValues;
    QwtPlot *_parent;

};
