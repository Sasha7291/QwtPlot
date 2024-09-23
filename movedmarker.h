#pragma once

#include <QObject>
#include <QwtPlotMarker>
#include <QwtPlotPicker>
#include <QwtSymbol>

class MonitorPlot;

class MovedMarker : public QObject
{
    Q_OBJECT

public:
    explicit MovedMarker(MonitorPlot *parent);
    ~MovedMarker();

    void hide();
    void reset(const double origin, const double end);
    void show();

signals:
    void deltaCalculated(const double xDelta, const double yDelta);

private slots:
    void moveMarker(const QPointF &pos);

private:
    std::unique_ptr<QwtPlotPicker> _picker;
    std::unique_ptr<QwtPlotMarker> _marker_1;
    std::unique_ptr<QwtPlotMarker> _marker_2;
    std::unique_ptr<QwtSymbol> _symbol_1;
    std::unique_ptr<QwtSymbol> _symbol_2;
    QwtPickerMachine *_lineDragStateMachine;
    MonitorPlot *_parent;

    double _origin;
    double _range;
    double _precision;

};
