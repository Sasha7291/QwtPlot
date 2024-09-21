#pragma once

#include <QwtPlotMarker>
#include <QwtPlotPicker>
#include <QwtPlotShapeItem>
#include <QwtSymbol>

class SelectedMarker : public QObject
{
    Q_OBJECT

public:
    enum class SelectMode
    {
        NoSelect = -1,
        PointSelect,
        HLineSelect,
        VLineSelect,
        RectSelect
    };

    explicit SelectedMarker(QwtPlot *parent);
    ~SelectedMarker();

    void setSelectMode(const SelectMode mode);

private slots:
    void clickPoint(const QPointF &pos);
    void dragRectangle(const QRectF &rect);

private:
    std::unique_ptr<QwtPlotPicker> _picker;
    std::unique_ptr<QwtPlotMarker> _marker;
    std::unique_ptr<QwtPlotShapeItem> _selectedRectangle;
    std::unique_ptr<QwtPlotShapeItem> _shadedArea;
    std::unique_ptr<QwtSymbol> _symbol;
    QwtPlot *_parent;
    QwtPickerMachine *_pointClickStateMachine;
    QwtPickerMachine *_rectangleDragStateMachine;

};
