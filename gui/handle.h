#ifndef HANDLE_H
#define HANDLE_H

#include <QGraphicsItem>

class Handle : public QGraphicsItem
{
public:
    Handle(QGraphicsItem *item, QPoint *point);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boudingRect() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    QGraphicsItem *m_item;
    QPoint *m_point;
    static QColor m_color;
    bool m_pressed;
};

#endif // HANDLE_H
