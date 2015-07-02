#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clockview.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_clockView(new ClockView)
{
    ui->setupUi(this);
    setCentralWidget(m_clockView);
}

void MainWindow::setClocks()
{
    qDebug() << "open Clock" << "\n";
    m_clockView->openClock(QFile(":/files/clock.svg"));
    qDebug() << "open Arrow" << "\n";
    m_clockView->openArrow(ClockView::ArrowType::Hour, QFile(":/files/hour_arrow.svg"));
    qDebug() << "open Arrow" << "\n";
    m_clockView->openArrow(ClockView::ArrowType::Minute, QFile(":/files/minute_arrow.svg"));
    qDebug() << "open Arrow" << "\n";
    m_clockView->openArrow(ClockView::ArrowType::Second, QFile(":/files/second_arrow.svg"));
    resize(m_clockView->sizeHint() + QSize(80, 80 + menuBar()->height()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
