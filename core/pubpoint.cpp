#include "pubpoint.h"
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QJsonObject>

PubPoint::PubPoint(QGraphicsItem *parent) :
    MapEntity(parent)
{
}

PUB_TYPE PubPoint::pubType() const
{
    return m_type;
}

void PubPoint::setPubType(PUB_TYPE type)
{
    if(m_type == type)
        return;
    m_type = type;
    emit pubTypeChanged(m_type);
}

bool PubPoint::load(const QJsonObject &jsonObject) {
    setObjectName( jsonObject["Name"].toString() );
    m_enName = jsonObject["Name_En"].toString();
    m_type = PUB_TYPE(jsonObject["Type"].toString().toInt());
    m_id = jsonObject["_id"].toInt();

    QJsonArray point = jsonObject["Outline"].toArray()[0].toArray()[0].toArray();
    m_center = QPoint(point[0].toInt(), point[1].toInt());
}

QRectF PubPoint::boundingRect() const
{
    const int padding = 5;
    QRectF rect(m_center, m_center);
    rect.adjust(-padding, -padding, padding, padding);
    return rect;
}

QPainterPath PubPoint::shape() const
{
    QPainterPath path;
    path.addEllipse(m_center, 5, 5);
    return path;
}

void PubPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    //if selected
    QColor color(22, 22, 22);
    QColor fillColor = (option->state & QStyle::State_Selected) ? color.darker(150) : color;
//    //if mouse over
//    if (option->state & QStyle::State_MouseOver)
//        fillColor = fillColor.lighter(125);

    painter->setBrush(fillColor);
    painter->drawEllipse(m_center, 5, 5);
}
