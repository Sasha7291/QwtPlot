#include "selectedmarker.h"

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
    , _pointSelect(nullptr)
    , _rectSelect(nullptr)
    , _symbol(nullptr)
    , _parent(parent)
    , _marker(std::make_unique<QwtPlotMarker>())
{
    this->_picker->setRubberBandPen(QPen(Qt::gray, 1.5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    this->_marker->setLinePen(QPen(Qt::black, 1.5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    this->_marker->attach(parent);

    this->setSelectMode(SelectMode::NoSelect);

    connect(this->_picker.get(), SIGNAL(selected(const QPointF&)), this, SLOT(moveMarker(const QPointF&)));
}

SelectedMarker::~SelectedMarker()
{
    delete _pointSelect;
    delete _rectSelect;
}

void SelectedMarker::setSelectMode(const SelectMode mode)
{
    switch (mode)
    {
    case SelectMode::NoSelect:
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_picker->setStateMachine(nullptr);
        this->_marker->setSymbol(nullptr);
        this->_marker->setVisible(false);
        break;

    case SelectMode::PointSelect:
        if (this->_pointSelect == nullptr)
            this->_pointSelect = new QwtPickerDragPointMachine;
        this->_picker->setStateMachine(this->_pointSelect);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        if (this->_symbol == nullptr)
            this->_symbol = std::make_unique<QwtSymbol>(QwtSymbol::Cross, Qt::red, QPen(Qt::red), QSize(10, 10));
        this->_marker->setLineStyle(QwtPlotMarker::Cross);
        break;

    case SelectMode::HLineSelect:
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_picker->setStateMachine(nullptr);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_marker->setSymbol(nullptr);
        this->_marker->setLineStyle(QwtPlotMarker::HLine);
        break;

    case SelectMode::VLineSelect:
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_picker->setStateMachine(nullptr);
        this->_picker->setRubberBand(QwtPicker::NoRubberBand);
        this->_marker->setSymbol(nullptr);
        this->_marker->setLineStyle(QwtPlotMarker::VLine);
        break;

    case SelectMode::RectSelect:
        if (this->_rectSelect == nullptr)
            this->_rectSelect = new QwtPickerDragRectMachine;
        this->_picker->setStateMachine(this->_rectSelect);
        this->_picker->setRubberBand(QwtPicker::RectRubberBand);
        this->_marker->setSymbol(nullptr);
        break;
    }

    this->_marker->setVisible(false);
}

void SelectedMarker::selectPoint(const QPointF &pos)
{
    this->_marker->setVisible(true);
    this->_marker->setValue(pos.x(), pos.y());
    this->_parent->replot();
}
