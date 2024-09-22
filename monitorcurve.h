#pragma once

#include <QColor>
#include <QwtPlotCurve>

class MonitorCurve : public QwtPlotCurve
{

public:
    explicit MonitorCurve(QwtPlot *parent, QColor color = Qt::red);

    void setData(const std::vector<double> &data);

private:
    QwtPlot *_parent;

};
