#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clockview.h"
#include <QFile>
#include <QFileDialog>
#include <QTimer>

const QString MainWindow::clockFilename = ":/files/clock.svg";
const QString MainWindow::hourArrowFilename = ":/files/hour_arrow.svg";
const QString MainWindow::minuteArrowFilename = ":/files/minute_arrow.svg";
const QString MainWindow::secondArrowFilename = ":/files/second_arrow.svg";

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
    m_clockView->openClock(QFile(clockFilename));
    m_clockView->openArrow(ClockView::ArrowType::Hour, QFile(hourArrowFilename));
    m_clockView->openArrow(ClockView::ArrowType::Minute, QFile(minuteArrowFilename));
    m_clockView->openArrow(ClockView::ArrowType::Second, QFile(secondArrowFilename));
    resize(m_clockView->sizeHint() + QSize(80, 80 + menuBar()->height()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_clock_triggered()
{
    m_clockView->openClock(QFile(getSvgFilename()));
}

QString MainWindow::getSvgFilename()
{
    // I really Know about static method QFileDialog::openFile
    // but for me it doesn't working
    // very similar bug https://code.google.com/p/marave/issues/detail?id=91
    QFileDialog dialog(this, tr("Open SVG file"),
                                m_currentPath, "SVG files (*.svg *.svgz *.svg.gz)");
    dialog.exec();
    QString fileName = dialog.selectedFiles().first();
    if (!fileName.isEmpty() && !fileName.startsWith(":/"))
    {
        m_currentPath = fileName;
    }
    return fileName;
}

void MainWindow::on_actionOpen_hour_arrow_triggered()
{
   m_clockView->openArrow(ClockView::ArrowType::Hour, QFile(getSvgFilename()));
}

void MainWindow::on_actionOpen_minute_arrow_triggered()
{
   m_clockView->openArrow(ClockView::ArrowType::Minute, QFile(getSvgFilename()));
}

void MainWindow::on_actionOpen_second_arrow_triggered()
{
   m_clockView->openArrow(ClockView::ArrowType::Second, QFile(getSvgFilename()));
}
