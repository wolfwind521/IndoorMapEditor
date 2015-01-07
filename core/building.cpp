#include "building.h"
#include "floor.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Building::Building(const QString & name, QGraphicsItem *parent)
    : PolygonEntity(name, parent), m_underfloors(0), m_groundFloors(0)
{
    m_color = QColor(247, 247, 247);
}

Building::Building(PolygonEntity &polygon)
{
    new (this) Building("");
    copy(polygon);
}

int Building::floorNum(){
    return m_underfloors + m_groundFloors;
}

bool Building::load(const QJsonObject &jsonObject)
{
    const QJsonValue & dataValue = jsonObject["data"];
    if(dataValue.isUndefined()){
        return false;
    }

    const QJsonObject & dataObject = dataValue.toObject();
    const QJsonValue & buildingValue = dataObject["building"];

    if(buildingValue.isUndefined()){
        return false;
    }

    const QJsonObject & buildingObject = buildingValue.toObject();
    PolygonEntity::load(buildingObject);

    m_underfloors = buildingObject["UnderFloors"].toInt();
    m_frontAngle = buildingObject["FrontAngle"].toDouble();
    m_defaultFloor = buildingObject["DefaultFloor"].toInt(1);
    m_groundFloors = buildingObject["GroundFloors"].toInt();
    m_postCode = buildingObject["Adcode"].toString();
    m_remark = buildingObject["Remark"].toString();
    m_floorsId = buildingObject["FloorsId"].toString();
    m_height = buildingObject["High"].toDouble();
    m_latitude = buildingObject["_yLat"].toDouble();
    m_longitude = buildingObject["_xLon"].toDouble();
    m_version = buildingObject["Version"].toInt();
    m_type = (BUILDING_TYPE) buildingObject["Type"].toString().toInt();
    m_key = buildingObject["_id"].toString();

    QVector<Floor*> allFloors;
    allFloors.resize(m_underfloors + m_groundFloors);

    QJsonArray floorsArray = dataObject["Floors"].toArray();
    for (int i = 0; i < floorsArray.size(); ++i) {
        QJsonObject floorObject = floorsArray[i].toObject();

        Floor * floor = new Floor(this);
        if(!floor->load(floorObject)) {
            //TODO: show some warning
        }
        int floorId = floor->id();
        if(floorId < 0) //underfloors
            allFloors[floorId + m_underfloors] = floor;
        else //groundfloors
            allFloors[floorId - 1 + m_underfloors] = floor;
    }

    for(int i = 0; i < allFloors.size(); i++)
        allFloors[i]->setParent(this);

    return true;
}

bool Building::save(QJsonObject &jsonObject)
{
    QJsonObject dataObject, buildingObject;

    dataObject["building"] = buildingObject;
    jsonObject["data"] = dataObject;

    PolygonEntity::save(buildingObject);
    buildingObject["UnderFloors"] = m_underfloors;
    buildingObject["FrontAngle"] = m_frontAngle;
    buildingObject["DefaultFloor"] = m_defaultFloor;
    buildingObject["GroundFloors"] = m_groundFloors;
    buildingObject["Adcode"] = m_postCode;
    buildingObject["Remark"] = m_remark;
    buildingObject["FloorsId"] = m_floorsId;
    buildingObject["High"] = m_height;
    buildingObject["_yLat"] = m_latitude;
    buildingObject["_xLon"] = m_longitude;
    buildingObject["Version"] = m_version;
    buildingObject["Type"] = QString::number(static_cast<int>(m_type));
    buildingObject["_id"] = m_key;

    QJsonArray floorArray;
    foreach(QObject* object, this->children()){
        if(object->metaObject()->className() == "Floor"){
            QJsonObject floorObject;
            static_cast<Floor*>(object)->save(floorObject);
            floorArray.append(floorObject);
        }
    }

    buildingObject["Floors"] = floorArray;
    return true;
}
