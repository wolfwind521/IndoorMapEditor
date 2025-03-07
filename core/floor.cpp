#include "floor.h"
#include "room.h"
#include "pubpoint.h"
#include "imagelayer.h"

int Floor::m_maxFloorId = 0;

Floor::Floor(QGraphicsItem *parent)
    : PolygonFeature(parent), m_height(0)
{
    m_color = QColor(193, 193, 193, 125);
    m_id = ++m_maxFloorId;
}

Floor::Floor(PolygonFeature &polygon)
{
    new (this) Floor();
    copy(polygon);
    m_id = m_maxFloorId++;
}

int Floor::generateId(){
    m_id = m_maxFloorId++;
    return m_id;
}

bool Floor::load(const QJsonObject &jsonObject) {

    PolygonFeature::load(jsonObject);
    m_height = jsonObject["High"].toDouble();
    m_id = jsonObject["_id"].toInt();
    if(m_id > m_maxFloorId){
        m_maxFloorId = m_id;
    }

    QJsonArray roomArray = jsonObject["Rooms"].toArray();
    for (int i = 0; i < roomArray.size(); ++i) {
        QJsonObject roomObject = roomArray[i].toObject();

        Room * room = new Room(this);
        if(!room->load(roomObject)) {
            //TODO: show some warning
        }
        room->setParent(this);
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

bool Floor::save(QJsonObject &jsonObject) const
{
    PolygonFeature::save(jsonObject);
    jsonObject["High"] = m_height;
    jsonObject["_id"] = m_id;

    //save the Rooms and pubPoints
    QJsonArray roomArray, pubArray, imageArray;
    foreach (QObject* object, this->children()) {
        QString className = object->metaObject()->className();
        if(className == "Room"){
            QJsonObject roomObject;
            static_cast<Room*>(object)->save(roomObject);
            roomArray.append(roomObject);
        }else if(className == "PubPoint"){
            QJsonObject pubObject;
            static_cast<PubPoint*>(object)->save(pubObject);
            pubArray.append(pubObject);
        }else if(className == "ImageLayer"){
            QJsonObject imageObject;
            static_cast<ImageLayer*>(object)->save(imageObject);
            imageArray.append(imageObject);
        }
    }
    jsonObject["Rooms"] = roomArray;
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

QList<Room*> Floor::getRooms() const{
    QList<Room*> Rooms;
    const QList<QGraphicsItem*> & children = this->childItems();
    QGraphicsItem* item;
    foreach(item, children){
        Room* room = dynamic_cast<Room*>(item);
        if(room != NULL){
            Rooms.append(room);
        }
    }
    return Rooms;
}


void Floor::transformFeature(const QTransform &mat)
{
    PolygonFeature::transformFeature(mat);

    const QList<QGraphicsItem*> & children = this->childItems();
    foreach (QGraphicsItem* item, children) {
        Feature *mapFeature = static_cast<Feature*>(item);
        if(mapFeature != NULL){
            mapFeature->transformFeature(mat);
        }
    }

}
