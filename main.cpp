#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(svgclock);
    MainWindow w;
    w.setClocks();
    w.show();

    return a.exec();
}
