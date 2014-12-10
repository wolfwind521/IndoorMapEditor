#include "floor.h"
#include "funcarea.h"
#include "pubpoint.h"

Floor::Floor(QGraphicsItem *parent)
    : PolygonEntity(parent)
{
    m_color = QColor(193, 193, 193);
}

bool Floor::load(const QJsonObject &jsonObject) {

    PolygonEntity::load(jsonObject);
    m_height = jsonObject["High"].toDouble();
    m_id = jsonObject["_id"].toInt();

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

bool Floor::save(QJsonObject &jsonObject)
{
    PolygonEntity::save(jsonObject);
    jsonObject["High"] = m_height;
    jsonObject["_id"] = m_id;

    //save the funcAreas
    QJsonArray funcArray;
    foreach (QObject* object, this->children()) {
        if(object->metaObject()->className() == "FuncArea"){
            QJsonObject funcObject;
            static_cast<FuncArea*>(object)->save(funcObject);
            funcArray.append(funcObject);
        }
    }
    jsonObject["FuncAreas"] = funcArray;

    //save the pubPoints
    QJsonArray pubArray;
    foreach(QObject* object, this->children()){
        if(object->metaObject()->className() == "PubPoint"){
            QJsonObject pubObject;
            static_cast<PubPoint*>(object)->save(pubObject);
            pubArray.append(pubObject);
        }
    }
    jsonObject["PubPoint"] = pubArray;
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
