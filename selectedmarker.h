#pragma once

#include <QwtPlotMarker>
#include <QwtPlotPicker>
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
    void selectPoint(const QPointF &pos);

private:
    std::unique_ptr<QwtPlotPicker> _picker;
    std::unique_ptr<QwtPlotMarker> _marker;
    std::unique_ptr<QwtSymbol> _symbol;
    QwtPlot *_parent;
    QwtPickerMachine *_pointSelect;
    QwtPickerMachine *_rectSelect;

};
