#include "linemarker.h"

#include <QPen>

LineMarker::LineMarker(QwtPlot *parent)
    : QwtPlotMarker()
{
    this->setLineStyle(QwtPlotMarker::HLine);
    this->setLinePen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    this->setValue(0.0, 0.0);
    this->attach(parent);
}
