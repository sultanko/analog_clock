#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

class ClockView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openClock();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClockView* m_clockView;
};

#endif // MAINWINDOW_H
