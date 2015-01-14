#include "building.h"
#include "floor.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

Building::Building(const QString & name, QGraphicsItem *parent)
    : PolygonEntity(name, parent), m_underFloors(0), m_groundFloors(0), m_defaultFloor(1), m_height(0)
{
    m_color = QColor(247, 247, 247);
}

Building::Building(PolygonEntity &polygon) {
    new (this) Building("");
    copy(polygon);
}

int Building::floorNum() const {
    return m_underFloors + m_groundFloors;
}

int Building::underFloors() const {
    return m_underFloors;
}

int Building::groundFloors() const {
    return m_groundFloors;
}

double Building::height() const {
    return m_height;
}

void Building::setHeight(double height) {
    m_height = height;
}

void Building::addFloor(Floor *floor) {
    if(floor->id() > 0) {
        m_groundFloors ++;
    } else{
        m_underFloors ++;
    }
    floor->setParentEntity(this);
    connect(floor, SIGNAL(idChanged(int,int)), this, SLOT(updateFloorIds(int,int)));
}

void Building::deleteFloor(Floor *floor) {
    if(floor->id() > 0) {
        m_groundFloors --;
    } else {
        m_underFloors --;
    }
    floor->setParent(NULL);
    floor->setParentItem(NULL);
    delete floor;
    floor = NULL;
}

void Building::updateFloorIds(int oldId, int newId) {
    if(oldId > 0 && newId < 0) {
        m_groundFloors --;
        m_underFloors ++;
    } else if (oldId < 0 && newId > 0){
        m_groundFloors ++;
        m_underFloors --;
    }
}

int Building::defaultFloor() const {
    return m_defaultFloor;
}

void Building::setDefaultFloor(int floorId) {
    m_defaultFloor = floorId;
}

bool Building::load(const QJsonObject &jsonObject) {
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

    m_underFloors = buildingObject["UnderFloors"].toInt();
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
    allFloors.resize(m_underFloors + m_groundFloors);

    QJsonArray floorsArray = dataObject["Floors"].toArray();
    for (int i = 0; i < floorsArray.size(); ++i) {
        QJsonObject floorObject = floorsArray[i].toObject();

        Floor * floor = new Floor(this);
        if(!floor->load(floorObject)) {
            //TODO: show some warning
        }
        int floorId = floor->id();
        if(floorId < 0) //underfloors
            allFloors[floorId + m_underFloors] = floor;
        else //groundfloors
            allFloors[floorId - 1 + m_underFloors] = floor;
    }

    for(int i = 0; i < allFloors.size(); i++)
        allFloors[i]->setParent(this);

    return true;
}

bool Building::save(QJsonObject &jsonObject) const
{
    QJsonObject dataObject, buildingObject;

    PolygonEntity::save(buildingObject);
    buildingObject["UnderFloors"] = m_underFloors;
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
        QString className = object->metaObject()->className();
        if( className == "Floor"){
            QJsonObject floorObject;
            static_cast<Floor*>(object)->save(floorObject);
            floorArray.append(floorObject);
        }
    }

    dataObject["Floors"] = floorArray;
    dataObject["building"] = buildingObject;
    jsonObject["data"] = dataObject;

    return true;
}

QVector<Floor*> Building::getFloors() {
    QVector<Floor*> floors;
    QList<QGraphicsItem*> children = this->childItems();
    QGraphicsItem* item;
    foreach(item, children) {
        floors.push_back(static_cast<Floor*>(item));
    }
    return floors;
}
