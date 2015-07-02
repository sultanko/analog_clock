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
#include <QFileInfo>
#include <qmath.h>

ClockView::ClockView(QWidget *parent) :
    QGraphicsView(parent),
    m_svgClock(nullptr)
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);

    // Create timer for update clock arrows
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000);

    // feature: with rectangle it's much easier to align arrows
    scene()->setSceneRect(0, 0, 400, 400);
}

void ClockView::openClock(const QFile &file)
{
    // Need to check if file isn't dir
    QFileInfo fileInfo(file);
    if (!fileInfo.exists() || fileInfo.isDir())
    {
        return;
    }
    QGraphicsScene* s = scene();
    // remove old svg clock
    if (m_svgClock != nullptr)
    {
        s->removeItem(m_svgClock);
        delete m_svgClock;
    }
    m_svgClock = new QGraphicsSvgItem(file.fileName());
    m_svgClock->setZValue(0);
    // Fill rectangle with clock
    qreal clockScale = s->sceneRect().height() / m_svgClock->boundingRect().height();
    m_svgClock->setScale(clockScale);
    s->addItem(m_svgClock);

    // Of course redraw
    redraw();
}

void ClockView::openArrow(ClockView::ArrowType arrowType, const QFile &file)
{
    QFileInfo fileInfo(file);
    if (!fileInfo.exists() || fileInfo.isDir())
    {
        return;
    }
    QGraphicsSvgItem* arrow = new QGraphicsSvgItem(file.fileName());
    // remove previous used arrow
    if (m_arrows.contains(arrowType))
    {
        scene()->removeItem(m_arrows[arrowType]);
        delete m_arrows[arrowType];
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
    // Get factor from svgviewer example
    // looks preety enough
    qreal factor = qPow(1.2, event->delta() / 240.0);
    scale(factor, factor);
    // resize our scene rect
    QRectF rect = sceneRect();
    scene()->setSceneRect(rect.x(), rect.y(), rect.height() * factor, rect.width() * factor);
    // based on new rect redraw elements
    redraw();
    event->accept();
}

void ClockView::rotateArrow(const ClockView::ArrowType &arrowType)
{
    QGraphicsItem* arrow = m_arrows.value(arrowType, nullptr);
    // If there is no svg file for this type of arrow
    if (arrow == nullptr)
    {
        return;
    }
    QTime currentTime = QTime::currentTime();
    qreal rotationAngle;
    // Scale factor is using to resize arrow depends on it's type
    qreal scaleFactor = 0.5 * m_svgClock->sceneBoundingRect().height() / arrow->boundingRect().height();
    switch (arrowType) {
    case Hour:
        rotationAngle = 30 * currentTime.hour() + 0.5 * currentTime.minute();
        scaleFactor *= 0.4;
        break;
    case Minute:
        rotationAngle = 6 * currentTime.minute() + 0.1 * currentTime.second();
        scaleFactor *= 0.6;
        break;
    case Second:
        rotationAngle = 6 * currentTime.second();
        scaleFactor *= 0.8;
        break;
    }
    QGraphicsScene* s = scene();
    // Okay, arrow are not so pretty and we need to rotate it
    // and align to center
    // Using rotate matrix for it
    qreal radRotation = qDegreesToRadians(rotationAngle);
    QTransform rotateMatrix(qCos(radRotation), qSin(radRotation), -qSin(radRotation), qCos(radRotation), 0, 0);
    QPoint shift = rotateMatrix.map(QPoint(arrow->boundingRect().width() / 2.0, arrow->boundingRect().height()));
    arrow->setPos(s->sceneRect().center().x() - shift.x() * scaleFactor, s->sceneRect().center().y() - shift.y() * scaleFactor);
    arrow->setRotation(rotationAngle);
    // Priority of view: Hour > Minute > Second > Clock
    arrow->setZValue(arrowType);
    arrow->setScale(scaleFactor);
}

void ClockView::redraw()
{
    resetTransform();
    qreal clockScale = sceneRect().height() / m_svgClock->boundingRect().height();
    if (clockScale != m_svgClock->scale())
    {
        m_svgClock->setScale(clockScale);
    }
    for (auto it = m_arrows.begin(); it != m_arrows.end(); ++it)
    {
        rotateArrow(it.key());
    }
}
