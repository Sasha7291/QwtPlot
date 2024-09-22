#include <QApplication>
#include <QMainWindow>

#include "abstractplot.h"
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
    // p.setParameters({
    //     { LinePlot::xOrigin, 0.0 },
    //     { LinePlot::yOrigin, 0.0 },
    //     { LinePlot::xRange, 100.0 },
    //     { LinePlot::yRange, 100.0 },
    //     { LinePlot::xSamples, 100u },
    //     { LinePlot::ySamples, 100u },
    //     { LinePlot::xAxisName, QString("x") },
    //     { LinePlot::yAxisName, QString("y") }
    // });

    for (int y = 0; y < 100; ++y)
        p.addData(values[y]);

    wnd.setCentralWidget(p.instance());
    wnd.resize(400, 400);
    wnd.show();

    return QApplication::exec();
}
