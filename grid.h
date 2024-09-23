#pragma once

#include <QPen>
#include <QwtPlotGrid>

class Grid : public QwtPlotGrid
{

public:
    explicit Grid(QwtPlot* parent)
        : QwtPlotGrid()
        , _parent(parent)
    {
        this->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
        this->enableX(true);
        this->enableXMin(false);
        this->enableY(true);
        this->enableYMin(false);
        this->attach(parent);
    }

private:
    QwtPlot *_parent;

};
