#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <QGraphicsView>

class QTimer;

class ClockView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ClockView(QWidget *parent = 0);

    void openClock();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent *event);

private:
    void paint();

private:
    QGraphicsRectItem* m_borderRect;
    QGraphicsItem *m_svgClock;
    QGraphicsItem *m_arrow;
    QTimer* m_timer;
};

#endif // CLOCKVIEW_H
