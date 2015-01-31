#include "floor.h"
#include "funcarea.h"
#include "pubpoint.h"
#include "imagelayer.h"

int Floor::m_maxFloorId = 0;

Floor::Floor(QGraphicsItem *parent)
    : PolygonEntity(parent), m_height(0)
{
    m_color = QColor(193, 193, 193, 125);
    m_id = ++m_maxFloorId;
}

Floor::Floor(PolygonEntity &polygon)
{
    new (this) Floor();
    copy(polygon);
    m_id = m_maxFloorId++;
}

bool Floor::load(const QJsonObject &jsonObject) {

    PolygonEntity::load(jsonObject);
    m_height = jsonObject["High"].toDouble();
    m_id = jsonObject["_id"].toInt();
    if(m_id > m_maxFloorId){
        m_maxFloorId = m_id;
    }

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

    QJsonArray imageArray = jsonObject["ImageLayer"].toArray();
    for(int i = 0; i < imageArray.size(); ++i){
        QJsonObject imageObject = imageArray[i].toObject();
        ImageLayer* imageLayer = new ImageLayer(this);
        if(!imageLayer->load(imageObject)){

        }
        imageLayer->setParent(this);
    }

    //for GaoDe maps. name is actually brief
    QString name = jsonObject["Name"].toString();
    if(!name[0].isUpper()){
        m_brief = name;
    }

    return true;
}

bool Floor::save(QJsonObject &jsonObject, double scale) const
{
    PolygonEntity::save(jsonObject, scale);
    jsonObject["High"] = m_height;
    jsonObject["_id"] = m_id;

    //save the funcAreas and pubPoints
    QJsonArray funcArray, pubArray, imageArray;
    foreach (QObject* object, this->children()) {
        QString className = object->metaObject()->className();
        if(className == "FuncArea"){
            QJsonObject funcObject;
            static_cast<FuncArea*>(object)->save(funcObject, scale);
            funcArray.append(funcObject);
        }else if(className == "PubPoint"){
            QJsonObject pubObject;
            static_cast<PubPoint*>(object)->save(pubObject, scale);
            pubArray.append(pubObject);
        }else if(className == "ImageLayer"){
            QJsonObject imageObject;
            static_cast<ImageLayer*>(object)->save(imageObject, scale);
            imageArray.append(imageObject);
        }
    }
    jsonObject["FuncAreas"] = funcArray;
    jsonObject["PubPoint"] = pubArray;
    jsonObject["ImageLayer"] = imageArray;
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

void Floor::resetMaxFloorId(){
    m_maxFloorId = 0;
}
