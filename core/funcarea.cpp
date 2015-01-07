#include "funcarea.h"
#include <QPainter>
#include <QApplication>

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent)
{
    m_color = QColor(239, 229, 217);
    setObjectName(tr("unnamed"));
}

FuncArea::FuncArea(PolygonEntity &polygon)
{
    new (this) FuncArea();
    copy(polygon);
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
    return true;
}

void FuncArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    PolygonEntity::paint(painter, option, widget);

    //paint the marker
    if(!m_center.isNull()){
        painter->setBrush(QColor(22, 22, 22));
        painter->drawEllipse(m_center, 3, 3);

        //paint the text
        painter->setPen(QPen());
        QFont font = QApplication::font();
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(m_center, objectName());
    }
}
