#include <QApplication>
#include <QMainWindow>

#include "plot.h"

int main( int argc, char* argv[] ) {
    QApplication app( argc, argv );
    QMainWindow wnd;

    std::vector<std::vector<double>> values(100, std::vector<double>(100));
    for (int x = 0; x < 100; ++x)
        for (int y = 0; y < 100; ++y)
            values[y][x] = x + y;

    Plot p(&wnd);
    p.start();
    p.setParameters({
        { Plot::xOrigin, 0.0 },
        { Plot::yOrigin, 0.0 },
        { Plot::xRange, 100.0 },
        { Plot::yRange, 100.0 },
        { Plot::xSamples, 100u },
        { Plot::ySamples, 100u },
        { Plot::xAxisName, QString("x") },
        { Plot::yAxisName, QString("y") }
    });

    for (int y = 0; y < 100; ++y)
        p.addData(values[y]);

    wnd.setCentralWidget(&p);
    wnd.resize(400, 400);
    wnd.show();

    return QApplication::exec();
}
