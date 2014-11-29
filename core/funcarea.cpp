#include "funcarea.h"

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent)
{
    m_color = QColor(239, 229, 217);
}

FUNC_TYPE FuncArea::funcType() const
{
    return m_type;
}

void FuncArea::setFuncType(const FUNC_TYPE type)
{
    if(m_type == type)
        return;
    m_type = type;
    emit funcTypeChanged(m_type);
}

bool FuncArea::load(const QJsonObject &jsonObject) {
    setObjectName( jsonObject["Name"].toString() );
    m_enName = jsonObject["Name_En"].toString();
    m_area = jsonObject["Area"].toDouble();
    m_type = FUNC_TYPE(jsonObject["Type"].toString().toInt());
    m_id = jsonObject["_id"].toInt();
    setOutline(jsonObject["Outline"].toArray()[0].toArray()[0].toArray());

    if(m_area == 0){
        computeArea();
    }
    if(m_center.isNull()){
        computeCenter();
    }
    return true;
}
