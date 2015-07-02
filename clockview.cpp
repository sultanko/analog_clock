#include "clockview.h"
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QPaintEvent>
#include <QTime>
#include <QSvgRenderer>
#include <qmath.h>

ClockView::ClockView(QWidget *parent) :
    QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);

//    m_timer = new QTimer(this);
//    m_timer->setInterval(1000);
//    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    // Prepare background check-board pattern
//    QPixmap tilePixmap(64, 64);
//    tilePixmap.fill(Qt::white);
//    QPainter tilePainter(&tilePixmap);
//    QColor color(220, 220, 220);
//    tilePainter.fillRect(0, 0, 32, 32, color);
//    tilePainter.fillRect(32, 32, 32, 32, color);
//    tilePainter.end();

//    setBackgroundBrush(tilePixmap);
    scene()->setSceneRect(0, 0, 400, 400);
}

void ClockView::openClock()
{
    paint();
}

void ClockView::paintEvent(QPaintEvent *event)
{
//    openClock();
    QGraphicsView::paintEvent(event);
}

void ClockView::wheelEvent(QWheelEvent *event)
{
    qreal factor = qPow(1.2, event->delta() / 240.0);
    scale(factor, factor);
    event->accept();
}

void ClockView::paint()
{
    qDebug() << "open file" << "\n";
    QFile aclock(":/files/clock.svg");
    QGraphicsScene* s = scene();
    s->clear();
    resetTransform();
    m_svgClock = new QGraphicsSvgItem(aclock.fileName());
    qDebug() << "opened file" << "\n";
    m_svgClock->setZValue(0);
    qDebug() << "Clock: " << m_svgClock->boundingRect().size() << "\n";
    qreal clockScale = s->sceneRect().height() / m_svgClock->boundingRect().height();
    m_svgClock->setScale(clockScale);
    s->addItem(m_svgClock);

    m_arrow = new QGraphicsSvgItem(QFile(":/files/arrow.svg").fileName());
    m_arrow->setZValue(1);

    QTime currentTime = QTime::currentTime();
    qreal rotationAngle = 6 * currentTime.minute();
    qreal radRotation = qDegreesToRadians(rotationAngle);
    QTransform rotateMatrix(qCos(radRotation), qSin(radRotation), -qSin(radRotation), qCos(radRotation), 0, 0);
    QPoint shift = rotateMatrix.map(QPoint(m_arrow->boundingRect().width() / 2.0, m_arrow->boundingRect().height()));
    m_arrow->setPos(s->sceneRect().center().x() - shift.x(), s->sceneRect().center().y() - shift.y());
    m_arrow->setRotation(rotationAngle);
    s->addItem(m_arrow);
//    QGraphicsRectItem* m_outlineItem = new QGraphicsRectItem(m_arrow->boundingRect());
//    QPen outline(Qt::black, 2, Qt::DashLine);
//    outline.setCosmetic(true);
//    m_outlineItem->setPen(outline);
//    m_outlineItem->setBrush(Qt::NoBrush);
//    m_outlineItem->setZValue(1);
//    m_outlineItem->setPos(s->sceneRect().center().x() + posX, s->sceneRect().center().y() + posY);
////    m_outlineItem->setTransformOriginPoint(m_outlineItem->scenePos());
//    m_outlineItem->setRotation(rotationAngle);
//    s->addItem(m_outlineItem);
}
