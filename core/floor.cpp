#include "floor.h"
#include "funcarea.h"
#include "pubpoint.h"

Floor::Floor(QGraphicsItem *parent)
    : PolygonEntity(parent)
{
    m_color = QColor(193, 193, 193);
}

bool Floor::load(const QJsonObject &jsonObject) {

    setObjectName( jsonObject["Name"].toString() );
    m_area = jsonObject["Area"].toDouble();
    m_height = jsonObject["High"].toDouble();
    m_id = jsonObject["_id"].toInt();
    setOutline(jsonObject["Outline"].toArray()[0].toArray()[0].toArray());

    QJsonArray funcArray = jsonObject["FuncAreas"].toArray();
    for (int i = 0; i < funcArray.size(); ++i) {
        QJsonObject funcObject = funcArray[i].toObject();

        FuncArea * funcArea = new FuncArea(this);
        if(!funcArea->load(funcObject)) {
            //TODO: show some warning
        }
        funcArea->setParent(this);
    }

    QJsonArray pubArray = jsonObject["PubPoint"].toArray();
    for (int i = 0; i < pubArray.size(); ++i) {
        QJsonObject pubObject = pubArray[i].toObject();

        PubPoint * pubPoint = new PubPoint(this);
        if(!pubPoint->load(pubObject)) {
            //TODO: show some warning
        }
        pubPoint->setParent(this);
    }
    return true;
}

double Floor::height() const
{
    return m_height;
}

void Floor::setHeight(double height)
{
    if(m_height == height)
        return;
    m_height = height;
    emit heightChanged(m_height);
}
