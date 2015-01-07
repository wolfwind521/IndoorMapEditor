#include "handle.h"
#include <QPainter>

QColor Handle::m_color = QColor::fromRgb(0, 0, 0);

Handle::Handle(QGraphicsItem *item, QPoint *point)
    :m_item(item), m_point(point), m_pressed(false)
{
    setZValue(100);
    setFlags(ItemIsSelectable | ItemIsMovable);
}

QRectF Handle::boudingRect() const{
    const int padding = 3;
    QRectF rect(*m_point, *m_point);
    rect.adjust(-padding, -padding, padding, padding);
    return rect;
}

void Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(m_color);
    painter->setBrush(m_color);

    QRectF rect = boundingRect();
    painter->drawEllipse(rect);
}

void Handle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_pressed = true;
    QGraphicsItem::mousePressEvent(event);
}

void Handle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_pressed = false;
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant Handle::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(change == ItemPositionChange && m_pressed){
        QPointF movement = value.toPoint() - pos();
        m_point->rx() += movement.x();
        m_point->ry() += movement.y();
    }
    return QGraphicsItem::itemChange(change, value);
}
