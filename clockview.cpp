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
    QGraphicsView(parent),
    m_svgClock(nullptr)
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

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

void ClockView::openClock(const QFile &file)
{
    if (!file.exists())
    {
        return;
    }
    QGraphicsScene* s = scene();
    if (m_svgClock != nullptr)
    {
        s->removeItem(m_svgClock);
    }
    m_svgClock = new QGraphicsSvgItem(file.fileName());
    m_svgClock->setZValue(0);
    qreal clockScale = s->sceneRect().height() / m_svgClock->boundingRect().height();
    m_svgClock->setScale(clockScale);
    s->addItem(m_svgClock);
    redraw();
}

void ClockView::openArrow(ClockView::ArrowType arrowType, const QFile &file)
{
    if (!file.exists())
    {
        return;
    }
    QGraphicsSvgItem* arrow = new QGraphicsSvgItem(file.fileName());
    if (m_arrows.contains(arrowType))
    {
        scene()->removeItem(m_arrows[arrowType]);
    }
    m_arrows.insert(arrowType, arrow);
    scene()->addItem(arrow);
    rotateArrow(arrowType);
}

void ClockView::paintEvent(QPaintEvent *event)
{
    redraw();
    QGraphicsView::paintEvent(event);
}

void ClockView::wheelEvent(QWheelEvent *event)
{
    qreal factor = qPow(1.2, event->delta() / 240.0);
    scale(factor, factor);
    event->accept();
}

void ClockView::rotateArrow(const ClockView::ArrowType &arrowType)
{
    QGraphicsItem* arrow = m_arrows.value(arrowType, nullptr);
    if (arrow == nullptr)
    {
        return;
    }
    QTime currentTime = QTime::currentTime();
    qreal rotationAngle;
    switch (arrowType) {
    case Hour:
        rotationAngle = 30 * currentTime.hour() + 0.5 * currentTime.minute();
        break;
    case Minute:
        rotationAngle = 6 * currentTime.minute() + 0.1 * currentTime.second();
        break;
    case Second:
        rotationAngle = 6 * currentTime.second();
        break;
    }
    QGraphicsScene* s = scene();
    qreal radRotation = qDegreesToRadians(rotationAngle);
    QTransform rotateMatrix(qCos(radRotation), qSin(radRotation), -qSin(radRotation), qCos(radRotation), 0, 0);
    QPoint shift = rotateMatrix.map(QPoint(arrow->boundingRect().width() / 2.0, arrow->boundingRect().height()));
    arrow->setPos(s->sceneRect().center().x() - shift.x(), s->sceneRect().center().y() - shift.y());
    arrow->setRotation(rotationAngle);
    arrow->setZValue(arrowType);
    qDebug() << "this" << "\n";

}

void ClockView::redraw()
{
    QGraphicsScene* s = scene();
    resetTransform();
    for (auto it = m_arrows.begin(); it != m_arrows.end(); ++it)
    {
        rotateArrow(it.key());
    }
}
