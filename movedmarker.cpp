#include "movedmarker.h"

#include "monitorplot.h"

#include <QPen>
#include <QwtPickerMachine>
#include <QwtScaleDiv>

MovedMarker::MovedMarker(MonitorPlot *parent)
    : QObject(parent)
    , _picker(std::make_unique<QwtPlotPicker>(
        parent->xBottom,
        parent->yLeft,
        QwtPicker::NoRubberBand,
        QwtPicker::AlwaysOn,
        parent->canvas())
    )
    , _marker_1(std::make_unique<QwtPlotMarker>())
    , _marker_2(std::make_unique<QwtPlotMarker>())
    , _symbol_1(std::make_unique<QwtSymbol>(
        QwtSymbol::XCross,
        QBrush(QColor(198, 149, 1)),
        QPen(QColor(198, 149, 1)),
        QSize(10, 10))
    )
    , _symbol_2(std::make_unique<QwtSymbol>(
        QwtSymbol::XCross,
        QBrush(QColor(198, 149, 1)),
        QPen(QColor(198, 149, 1)),
        QSize(10, 10))
    )
    , _lineDragStateMachine(new QwtPickerDragPointMachine)
    , _parent(parent)
    , _precision(5.0)
{
    this->_picker->setRubberBandPen(QPen(Qt::NoPen));

    this->_marker_1->setLinePen(QPen(QColor(198, 149, 1), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    this->_marker_1->setLineStyle(QwtPlotMarker::VLine);
    this->_marker_1->setSymbol(this->_symbol_1.get());
    this->_marker_1->attach(parent);
    this->_marker_2->setLinePen(QPen(QColor(198, 149, 1), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    this->_marker_2->setLineStyle(QwtPlotMarker::VLine);
    this->_marker_2->setSymbol(this->_symbol_2.get());
    this->_marker_2->attach(parent);

    this->_picker->setStateMachine(this->_lineDragStateMachine);

    connect(this->_picker.get(), SIGNAL(moved(const QPointF&)), this, SLOT(moveMarker(const QPointF&)));
}

MovedMarker::~MovedMarker()
{
    delete _lineDragStateMachine;
}

void MovedMarker::hide()
{
    this->_marker_1->hide();
    this->_marker_2->hide();
}

void MovedMarker::reset(const double origin, const double end)
{
    this->_marker_1->setValue(
        (end - origin) / 3 + origin,
        0.0
    );
    this->_marker_2->setValue(
        2 * (end - origin) / 3 + origin,
        0.0
    );
    this->_precision = 0.05 * (end - origin);
    this->_origin = origin;
    this->_range = end - origin;
}

void MovedMarker::show()
{
    this->_marker_1->show();
    this->_marker_2->show();
}

void MovedMarker::moveMarker(const QPointF &pos)
{
    if (pos.x() < this->_origin + 0.005 * this->_range
        || pos.x() > (this->_origin + this->_range) - 0.005 * this->_range)
        return;

    if (abs(pos.x() - this->_marker_1->xValue()) < this->_precision)
    {
        this->_marker_1->setValue(
            _parent->curve()->sample((pos.x() - this->_origin) / this->_range * _parent->curve()->dataSize())
        );
        this->_parent->replot();

        emit deltaCalculated(
            abs(this->_marker_1->xValue() - this->_marker_2->xValue()),
            abs(this->_marker_1->yValue() - this->_marker_2->yValue())
        );
    }
    else if (abs(pos.x() - this->_marker_2->xValue()) < this->_precision)
    {
        this->_marker_2->setValue(
            _parent->curve()->sample((pos.x() - this->_origin) / this->_range * _parent->curve()->dataSize())
        );
        this->_parent->replot();

        emit deltaCalculated(
            abs(this->_marker_1->xValue() - this->_marker_2->xValue()),
            abs(this->_marker_1->yValue() - this->_marker_2->yValue())
        );
    }
}
