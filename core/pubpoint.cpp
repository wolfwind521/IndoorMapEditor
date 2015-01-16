#include "pubpoint.h"
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QJsonObject>
#include <QApplication>

PubPoint::PubPoint(QGraphicsItem *parent) :
    MapEntity(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
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

bool PubPoint::load(const QJsonObject &jsonObject)
{
    MapEntity::load(jsonObject);
    m_type = static_cast<PUB_TYPE>(jsonObject["Type"].toString().toInt());
    m_id = jsonObject["_id"].toInt();

    QJsonArray point = jsonObject["Outline"].toArray()[0].toArray()[0].toArray();
    m_center = QPoint(point[0].toInt(), -point[1].toInt());
    return true;
}

bool PubPoint::save(QJsonObject &jsonObject) const
{
    MapEntity::save(jsonObject);
    jsonObject["Type"] = static_cast<int>(m_type);
    jsonObject["_id"] = m_id;

    QJsonArray point;
    point.append(m_center.x());
    point.append(-m_center.y());

    QJsonArray array0, array1;
    array0.append(point);
    array1.append(array0);

    jsonObject["Outline"] = array1;
    return true;
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
    QColor color(125, 125, 125);
    QColor fillColor = (option->state & QStyle::State_Selected) ? color.darker(150) : color;
//    //if mouse over
//    if (option->state & QStyle::State_MouseOver)
//        fillColor = fillColor.lighter(125);

    painter->setBrush(fillColor);
    painter->setPen(QPen(fillColor.darker(),1));
    painter->drawEllipse(m_center, 5, 5);

    painter->setPen(QPen());
    QFont font = QApplication::font("DocumentView");
    font.setPixelSize(font.pointSize());
    //font.setPixelSize(12);
    painter->setFont(font);
    QRect fontRect = QFontMetrics(font).boundingRect(objectName());
    int width = fontRect.width();
    int height = fontRect.height();
    painter->drawText(QPoint(m_center.x()-width/2.0, m_center.y() - height/2.0), objectName());
}
