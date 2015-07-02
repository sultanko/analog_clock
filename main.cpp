#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(svgclock);
    MainWindow w;
    qDebug() << "create Window" << "\n";
    w.openClock();
    qDebug() << "open Window" << "\n";
    w.show();
    qDebug() << "show Window" << "\n";

    return a.exec();
}
