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
    m_svgClock->setFlags(QGraphicsItem::ItemClipsToShape);
    m_svgClock->setCacheMode(QGraphicsItem::NoCache);
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
    arrow->setFlags(QGraphicsItem::ItemClipsToShape);
    m_arrows.insert(arrowType, arrow);
    // Priority of view: Hour > Minute > Second > Clock
    arrow->setZValue(arrowType);
    scene()->addItem(arrow);
    rotateArrow(arrowType);
}

void ClockView::paintEvent(QPaintEvent *event)
{
    redraw();
    QGraphicsView::paintEvent(event);
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
    // Okay, arrow are not so pretty and we need to rotate it
    // and align to center
    // Using rotate matrix for it
    qreal radRotation = qDegreesToRadians(rotationAngle);
    QTransform rotateMatrix(qCos(radRotation), qSin(radRotation), -qSin(radRotation), qCos(radRotation), 0, 0);
    QPoint shift = rotateMatrix.map(QPoint(arrow->boundingRect().width() / 2.0, arrow->boundingRect().height()));
    const QRectF boundingRect = m_svgClock->sceneBoundingRect();
    arrow->setPos(boundingRect.center().x() - shift.x(), boundingRect.center().y() - shift.y());
    arrow->setRotation(rotationAngle);
}

void ClockView::redraw()
{
    resetTransform();
    qreal clockScale = qMin((qreal)(size().height() - 10) / sceneRect().height(),
                            (qreal)(size().width() - 10) / sceneRect().width());
    scale(clockScale, clockScale);
    for (auto it = m_arrows.begin(); it != m_arrows.end(); ++it)
    {
        rotateArrow(it.key());
    }
}
