#include <QApplication>
#include <QMainWindow>

#include "abstractplot.h"
#include "lineplot.h"
#include "monitorplot.h"

int main( int argc, char* argv[] ) {
    QApplication app( argc, argv );
    QMainWindow wnd;

    std::vector<std::vector<double>> values(100, std::vector<double>(100));
    for (int x = 0; x < 100; ++x)
        for (int y = 0; y < 100; ++y)
            values[y][x] = x + y;

    AbstractPlot<MonitorPlot> p(&wnd);
    // p.start();
    p.setParameters({
        { MonitorPlot::origin, 0.0 },
        { MonitorPlot::end, 1.0 },
        { MonitorPlot::end, 100u },
        { MonitorPlot::xAxisName, QString("x") },
        { MonitorPlot::yAxisName, QString("y") },
        { MonitorPlot::curveNames, QStringList({"", ""})}
    });

    for (int y = 0; y < 100; ++y)
        p.addData(values[y]);

    wnd.setCentralWidget(p.instance());
    wnd.resize(400, 200);
    wnd.show();

    return QApplication::exec();
}
