#include "selectedmarker.h"

#include <QPainterPath>
#include <QwtPickerMachine>
#include <QwtPlot>

SelectedMarker::SelectedMarker(QwtPlot *parent)
    : QObject(parent)
    , _picker(std::make_unique<QwtPlotPicker>(
        parent->xBottom,
        parent->yLeft,
        QwtPicker::NoRubberBand,
        QwtPicker::ActiveOnly,
        parent->canvas())
    )
    , _marker(std::make_unique<QwtPlotMarker>())
    , _symbol(nullptr)
    , _selectedRectangle(std::make_unique<QwtPlotShapeItem>())
    , _shadedArea(std::make_unique<QwtPlotShapeItem>())
    , _pointClickStateMachine(nullptr)
    , _rectangleDragStateMachine(nullptr)
    , _parent(parent)
{
    this->_picker->setRubberBandPen(QPen(Qt::gray, 1.5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

    this->_marker->setLinePen(QPen(Qt::red, 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    this->_marker->attach(parent);

    this->_selectedRectangle->setBrush(QBrush(Qt::NoBrush));
    this->_selectedRectangle->setPen(QPen(Qt::red, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    this->_selectedRectangle->attach(parent);

    this->_shadedArea->setBrush(QBrush(QColor(128, 128, 128, 64), Qt::SolidPattern));
    this->_shadedArea->setPen(QPen(Qt::NoPen));
    this->_shadedArea->attach(parent);

    this->setSelectMode(SelectMode::HLineSelect);

    connect(this->_picker.get(), SIGNAL(selected(const QPointF&)), this, SLOT(clickPoint(const QPointF&)));
    connect(this->_picker.get(), SIGNAL(selected(const QRectF&)), this, SLOT(dragRectangle(const QRectF&)));
}

SelectedMarker::~SelectedMarker()
{
    delete _pointClickStateMachine;
    delete _rectangleDragStateMachine;
}

void SelectedMarker::setSelectMode(const SelectMode mode)
{
    switch (mode)
    {
    case SelectMode::NoSelect:
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_picker->setStateMachine(nullptr);
        this->_marker->setSymbol(nullptr);
        this->_marker->hide();
        break;

    case SelectMode::PointSelect:
        if (this->_pointClickStateMachine == nullptr)
            this->_pointClickStateMachine = new QwtPickerClickPointMachine;
        this->_picker->setStateMachine(this->_pointClickStateMachine);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        if (this->_symbol == nullptr)
            this->_symbol = std::make_unique<QwtSymbol>(QwtSymbol::Cross, Qt::red, QPen(Qt::red), QSize(10, 10));
        this->_marker->setLineStyle(QwtPlotMarker::Cross);
        break;

    case SelectMode::HLineSelect:
        if (this->_pointClickStateMachine == nullptr)
            this->_pointClickStateMachine = new QwtPickerClickPointMachine;
        this->_picker->setStateMachine(this->_pointClickStateMachine);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_marker->setSymbol(nullptr);
        this->_marker->setLineStyle(QwtPlotMarker::HLine);
        break;

    case SelectMode::VLineSelect:
        if (this->_pointClickStateMachine == nullptr)
            this->_pointClickStateMachine = new QwtPickerClickPointMachine;
        this->_picker->setStateMachine(this->_pointClickStateMachine);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_marker->setSymbol(nullptr);
        this->_marker->setLineStyle(QwtPlotMarker::VLine);
        break;

    case SelectMode::RectSelect:
        if (this->_rectangleDragStateMachine == nullptr)
            this->_rectangleDragStateMachine = new QwtPickerDragRectMachine;
        this->_picker->setStateMachine(this->_rectangleDragStateMachine);
        this->_picker->setRubberBand(QwtPicker::RectRubberBand);
        this->_marker->setSymbol(nullptr);
        break;
    }

    this->_marker->hide();
    this->_selectedRectangle->hide();
    this->_shadedArea->hide();
}

void SelectedMarker::clickPoint(const QPointF &pos)
{
    this->_marker->setValue(pos.x(), pos.y());
    this->_marker->show();
    this->_parent->replot();
}

void SelectedMarker::dragRectangle(const QRectF &rect)
{
    // рисуем выделенный прямоугольник
    this->_selectedRectangle->setRect(rect);

    // затеняем область вне выделенного прямоугольника
    QPainterPath inner;
    QPainterPath outter;
    inner.addRect(rect);
    outter.addRect(_parent->canvas()->rect());
    this->_shadedArea->setShape(outter.subtracted(inner));

    this->_selectedRectangle->show();
    this->_shadedArea->show();
    this->_parent->replot();
}
