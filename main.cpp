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
    p.setParameters(
        0.0,
        0.0,
        100.0,
        100.0,
        100,
        100,
        "x",
        "y"
    );

    for (int y = 0; y < 50; ++y)
        p.addData(values[y]);

    wnd.setCentralWidget(&p);
    wnd.resize(400, 400);
    wnd.show();

    return QApplication::exec();
}
