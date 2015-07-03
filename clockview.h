#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <QGraphicsView>
#include <QHash>

class QTimer;
class QFile;

class ClockView : public QGraphicsView
{
    Q_OBJECT
public:
    enum ArrowType { Second = 1, Minute = 2, Hour = 3};
    explicit ClockView(QWidget *parent = 0);

    void openClock(const QFile& file);
    void openArrow(ArrowType arrowType, const QFile& file);
    QTimer* m_timer;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event);

private:
    void rotateArrow(const ArrowType& arrowType);
    void redraw();

private:
    QGraphicsItem *m_svgClock;
    QHash<ArrowType, QGraphicsItem*> m_arrows;
};

#endif // CLOCKVIEW_H
