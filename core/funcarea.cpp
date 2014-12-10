#include "funcarea.h"
#include <QPainter>

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent)
{
    m_color = QColor(239, 229, 217);
}

FUNC_TYPE FuncArea::funcType() const {
    return m_type;
}

void FuncArea::setFuncType(const FUNC_TYPE type) {
    if(m_type == type)
        return;
    m_type = type;
    emit funcTypeChanged(m_type);
}

bool FuncArea::load(const QJsonObject &jsonObject) {
    PolygonEntity::load(jsonObject);

    m_type = static_cast<FUNC_TYPE>(jsonObject["Type"].toString().toInt());
    m_id = jsonObject["_id"].toInt();
    return true;
}

bool FuncArea::save(QJsonObject &jsonObject) {
    PolygonEntity::save(jsonObject);

    jsonObject["Type"] = QString::number(static_cast<int>(m_type));
    jsonObject["_id"] = m_id;
}

void FuncArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    PolygonEntity::paint(painter, option, widget);
    painter->setBrush(QColor(22, 22, 22));
    painter->drawEllipse(m_center, 3, 3);
    painter->setPen(QPen());
    painter->drawText(m_center, objectName());
}
