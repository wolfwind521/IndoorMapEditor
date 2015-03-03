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
    return childItems().size();
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
    m_type =  buildingObject["Type"].toString();
    m_key = buildingObject["_id"].toString();
    m_address = buildingObject["Address"].toString();
    m_time = buildingObject["Time"].toString();
    m_tel = buildingObject["Tel"].toString();

    //for GaoDe maps. the remark is the floor names
    QStringList floorNames;
    if(!m_remark.isEmpty() && !m_remark.isNull()){
        floorNames = m_remark.split("\"").at(4).split(",");
    }

    QStringList floorsId = m_floorsId.split(",");

    QJsonArray floorsArray = dataObject["Floors"].toArray();
    QVector<Floor*> allFloors;
    allFloors.resize(floorsArray.size());

    for (int i = 0; i < floorsArray.size(); ++i) {
        QJsonObject floorObject = floorsArray[i].toObject();

        Floor * floor = new Floor(this);
        if(!floor->load(floorObject)) {
            //TODO: show some warning
        }
        int floorId = floor->id();
        int realId = floorsId.indexOf(QString::number(floorId));
        allFloors[realId] = floor;
        if(!floorNames.isEmpty()){
            floor->setObjectName(floorNames.at(realId));
        }
    }

    for(int i = 0; i < allFloors.size(); i++)
        allFloors[i]->setParent(this);


    //compute the fron angle
     m_frontAngle = buildingObject["FrontAngle"].toDouble();
     if(m_frontAngle == 0){
         QPointF dir;
         if(!m_outline.empty()){
             dir = computeMainDir();
         }else{
            Floor *floor = getFloorById(1);
            dir = floor->computeMainDir();
         }
         m_frontAngle = atan(dir.y()/dir.x());
     }

    return true;
}

bool Building::save(QJsonObject &jsonObject, double scale) const
{
    QJsonObject dataObject, buildingObject;

    PolygonEntity::save(buildingObject, scale);
    buildingObject["UnderFloors"] = m_underFloors;
    buildingObject["FrontAngle"] = m_frontAngle;
    buildingObject["DefaultFloor"] = m_defaultFloor;
    buildingObject["GroundFloors"] = m_groundFloors;
    buildingObject["Adcode"] = m_postCode;
    buildingObject["Remark"] = m_remark;
    buildingObject["High"] = m_height;
    buildingObject["_yLat"] = m_latitude;
    buildingObject["_xLon"] = m_longitude;
    buildingObject["Version"] = m_version;
    buildingObject["Type"] =m_type;
    buildingObject["_id"] = m_key;
    buildingObject["Address"] = m_address;
    buildingObject["Time"] = m_time;
    buildingObject["Tel"] = m_tel;

    QString floorsId;
    QJsonArray floorArray;
    foreach(QObject* object, this->children()){
        QString className = object->metaObject()->className();
        if( className == "Floor"){
            QJsonObject floorObject;
            Floor* floor = static_cast<Floor*>(object);
            floor->save(floorObject, scale);
            floorsId += QString::number(floor->id()) + ",";
            floorArray.append(floorObject);
        }
    }
    floorsId.chop(1);
    buildingObject["FloorsId"] = floorsId;
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

Floor* Building::getFloorById(int id){
    QList<QGraphicsItem*> children = this->childItems();
    QGraphicsItem* item;
    foreach(item, children) {
        Floor *floor = static_cast<Floor*>(item);
        if(floor->id() == id){
            return floor;
        }
    }
    return NULL;
}

double Building::latitude() const{
    return m_latitude;
}

void Building::setLatitude(double lat){
    if(lat != m_latitude){
        m_latitude = lat;
        //TODO emit signal
    }
}

double Building::longitude() const{
    return m_longitude;
}

void Building::setLongitude(double lng) {
    if(lng != m_longitude){
        m_longitude = lng;
        //TODO emit signal
    }
}

const QString & Building::address() const{
    return m_address;
}

void Building::setAddress(const QString &ad){
    if(ad != m_address){
        m_address = ad;
        //TODO emit signal
    }
}

const QString & Building::postCode() const{
    return m_postCode;
}

void Building::setPostCode(const QString &pc){
    if(pc != m_postCode){
        m_postCode = pc;
        //TODO emit signal
    }
}

const QString & Building::time() const{
    return m_time;
}

void Building::setTime(const QString &time){
    if(time != m_time){
        m_time = time;
        //TODO emit signal
    }
}

const QString & Building::tel() const{
    return m_tel;
}

void Building::setTel(const QString &tel){
    if(tel != m_tel){
        m_tel = tel;
        //TODO emit signal
    }
}
