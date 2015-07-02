#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>

class ClockView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setClocks();
    ~MainWindow();

private slots:
    void on_actionOpen_clock_triggered();

    void on_actionOpen_hour_arrow_triggered();

    void on_actionOpen_minute_arrow_triggered();

    void on_actionOpen_second_arrow_triggered();

private:
    QString getSvgFilename();

private:
    Ui::MainWindow *ui;
    ClockView* m_clockView;
    QString m_currentPath;

    static const QString clockFilename;
    static const QString hourArrowFilename;
    static const QString minuteArrowFilename;
    static const QString secondArrowFilename;
};

#endif // MAINWINDOW_H
