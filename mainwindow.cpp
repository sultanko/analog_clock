#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clockview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_clockView(new ClockView)
{
    ui->setupUi(this);
    setCentralWidget(m_clockView);
}

void MainWindow::openClock()
{
    m_clockView->openClock();
    resize(m_clockView->sizeHint() + QSize(80, 80 + menuBar()->height()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
