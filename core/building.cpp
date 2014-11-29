#include "building.h"
#include "floor.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Building::Building(const QString & name, QGraphicsItem *parent)
    : PolygonEntity(name, parent)
{
    m_color = QColor(247, 247, 247);
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

    m_underfloors = buildingObject["UnderFloors"].toInt();
    m_frontAngle = buildingObject["FrontAngle"].toDouble();
    m_defaultFloor = buildingObject["DefaultFloor"].toInt(1);
    setObjectName( buildingObject["Name"].toString() );
    m_enName = buildingObject["Name_En"].toString();
    m_area = buildingObject["Area"].toDouble();
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
    setOutline(buildingObject["Outline"].toArray()[0].toArray()[0].toArray());

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
