#include "mapentity.h"

MapEntity::MapEntity(QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
}

MapEntity::MapEntity(const QString &name, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    setObjectName(name);
}

//QString MapEntity::name() const
//{
//    return m_name;
//}

//void MapEntity::setName(const QString & name)
//{
//    if(m_name == name)
//        return;
//    m_name = name;
//    emit nameChanged(m_name);
//}

int MapEntity::id() const
{
    return m_id;
}

void MapEntity::setId(const int id)
{
    if(m_id == id)
        return;
    m_id = id;
    emit idChanged(m_id);
}

const QString & MapEntity::enName() const
{
    return m_enName;
}

void MapEntity::setEnName(const QString & name){
    if( m_enName == name)
        return;
    m_enName = name;
    emit enNameChanged(m_enName);
}

const QPointF & MapEntity::center() const
{
    return m_center;
}

void MapEntity::setCenter(const QPointF & center)
{
    if(m_center == center)
        return;
    m_center = center;
    emit centerChanged(m_center);
}

bool MapEntity::load(const QJsonObject &jsonObject)
{
    setObjectName( jsonObject["Name"].toString() );
    m_enName = jsonObject["Name_en"].toString();
}

bool MapEntity::save(QJsonObject &jsonObject) const
{
    jsonObject["Name"] = objectName();
    jsonObject["Name_en"] = m_enName;
}

QRectF MapEntity::boundingRect() const
{
    return QRectF(m_center, m_center);
}

void MapEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
