#pragma once

#include <QwtPlotCurve>

class LineCurve : public QwtPlotCurve
{

public:
    explicit LineCurve(QwtPlot *parent);

    void setLineData(
        const std::vector<double> &data,
        const unsigned int lineNumber
    );
    inline void setParameters(
        const double max,
        const double min,
        const unsigned int samples
    )
    {
        this->_max = max;
        this->_min = min;
        this->_samples = samples;
    }

private:
    QwtPlot *_parent;

    double _max;
    double _min;
    unsigned int _samples;

};
