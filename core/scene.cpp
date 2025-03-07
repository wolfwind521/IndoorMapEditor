#include "scene.h"
#include "../tool/toolmanager.h"
#include "../tool/abstracttool.h"
#include "feature.h"
#include "building.h"
#include "floor.h"
#include "imagelayer.h"
#include "room.h"
#include "pubpoint.h"
#include <math.h>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent), m_selectable(true), m_root(NULL), m_building(NULL), m_curFloor(NULL), m_scaleSum(0.0), m_scaleNum(0), m_curScale(1.0)
{
    createRoot();
}

void Scene::reset(){
    QGraphicsScene::clear();
    Floor::resetMaxFloorId();
    m_building = NULL;
    createRoot();
    setBuilding(new Building(tr("未命名建筑")));
    setSelectedLayer(m_building);
    update();
}

Feature *Scene::root() const{
    return m_root;
}

void Scene::createRoot(){
    m_root = new Feature(tr("root")); //add a root node
    addItem(m_root);
}

Building* Scene::building() const{
    return m_building;
}

void Scene::setBuilding(Building *building)
{
    if(m_building == building)
        return;
    if(m_building != NULL) //delete the old one
        delete m_building;

    m_building = building;
    m_building->setParentItem(m_root);
    m_building->setParent(m_root);

}

PolygonFeature* Scene::createPolygonByContext() {
    Feature *curLayer = currentLayer();
    if(curLayer){
        if(curLayer->isClassOf("Building")){
            Building *building = dynamic_cast<Building*>(curLayer);
            if(building->outline().empty())
                return building;
            else
            {
                return addFloor(new Floor(m_building));
            }
        }
        else if(curLayer->isClassOf("Floor")){
            Floor *floor = dynamic_cast<Floor *>(curLayer);
            if(floor->outline().empty())
                return floor;
            else{
                return addRoom(new Room(floor));
            }
        }
        else{
            return addRoom(new Room(m_curFloor));
        }
    }
}

void Scene::deletePolygonByContext(PolygonFeature *feature) {
    if(feature->isClassOf("Floor")){
        m_building->deleteFloor(static_cast<Floor*>(feature));
    }else{
        delete feature;
        feature = NULL;
    }
}

Room* Scene::addRoom(Room *room){
    if(m_curFloor != NULL){
        room->setParentFeature(m_curFloor);
        emit buildingChanged();
        return room;
    }

}

Floor* Scene::addFloor(Floor *floor){
    if(m_building != NULL){
        if(floor)
            m_building->addFloor(floor);
        else{
            floor = new Floor();
            m_building->addFloor(floor);
        }
        m_curFloor = floor;
        setSelectedLayer(floor);
        emit buildingChanged();
        return floor;
    }

}

void Scene::addPubPoint(PubPoint *pubPoint){
    if(m_curFloor != NULL){
        pubPoint->setParentFeature(m_curFloor);
    }else{
        pubPoint->setParentFeature(m_building);
    }
    emit buildingChanged();
}

void Scene::addImageLayer(ImageLayer *imageLayer) {
//    if(m_curFloor != NULL) {
//        imageLayer->setParentFeature(m_curFloor);
//    }else{

//    }
    Floor *floor = new Floor();
    imageLayer->setParentFeature(floor);
    addFloor(floor);
}

void Scene::mousePressEvent( QGraphicsSceneMouseEvent *event ){
    ToolManager::instance()->currentTool().mousePressEvent(event);
    if(m_selectable && event->button() == Qt::LeftButton)
        QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ){
    ToolManager::instance()->currentTool().mouseReleaseEvent(event);
    if(m_selectable && event->button() == Qt::LeftButton)
        QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event ){
    if(m_selectable)
        QGraphicsScene::mouseMoveEvent(event);
    ToolManager::instance()->currentTool().mouseMoveEvent(event);
}

bool Scene::event(QEvent *event){

    if(event->type() == QEvent::FontChange){
        emit fontChanged(font());
    }
    return QGraphicsScene::event(event);

}

void Scene::setSelectable(bool b){
    m_selectable = b;
}

void Scene::convertSelectedToBuilding(){
    PolygonFeature* selectedFeature = static_cast<PolygonFeature*>(selectedItems().at(0));
    m_building->copy(*selectedFeature);
    deleteMapFeature(selectedFeature);
    emit buildingChanged();
}

void Scene::convertSelectedToFloor(){
    PolygonFeature* selectedFeature = static_cast<PolygonFeature*>(selectedItems().at(0));
    if(m_curFloor != NULL && m_curFloor->outline().empty()){
        m_curFloor->setOutline(selectedFeature->outline());
    }else{
        addFloor(new Floor(*selectedFeature));
    }
     deleteMapFeature(selectedFeature);
}

void Scene::convertSelectedToRoom(){
    PolygonFeature* selectedFeature = static_cast<PolygonFeature*>(selectedItems().at(0));
    addRoom(new Room(*selectedFeature));
    selectedFeature->outline().clear();
}

void Scene::deleteSelectedItems(){
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*> realItems;
    QList<QGraphicsItem*>::iterator iter;
    for(iter = items.begin(); iter != items.end(); iter++){
        (*iter)->setSelected(false);
        if(!qgraphicsitem_cast<QGraphicsTextItem*>(*iter)){
            realItems.append(*iter);
        }
    }
    foreach (QGraphicsItem* item, realItems) {
        Feature *mapFeature = dynamic_cast<Feature *>(item);
        if(mapFeature)
            deleteMapFeature(mapFeature);
    }
    update();
}

void Scene::deleteSelectedLayers(){
    bool isBuilding = false;
    foreach(QGraphicsItem* item, m_selectedLayers){
        if(item){
            Feature *mapFeature = qgraphicsitem_cast<Feature*>(item);
            if(mapFeature->isClassOf("Building"))
                isBuilding = true;
            removeMapFeature(mapFeature);
            delete item;
            item = NULL;
        }
    }
    if(isBuilding){
        reset();
    }
    clearSelectedLayers();
}

void Scene::deleteMapFeature(Feature *feature){

    //如果是floor或building，仅清空其轮廓
    if(feature->isClassOf("Floor")||feature->isClassOf("Building")){
        PolygonFeature* poly = dynamic_cast<PolygonFeature*>(feature);
        poly->outline().clear();
    }else{ //否则直接删除
        removeMapFeature(feature);
        delete feature;
        feature = NULL;
    }
    emit buildingChanged();
}

void Scene::removeMapFeature(Feature *feature){
    feature->setParent(NULL);
    feature->setParentItem(NULL);
}

bool Scene::showFloor(int floorId) {
    QObject *object;
    bool found = false;
    QVector<Floor*> floors = building()->getFloors();
    if(floors.empty())
        return false;
    foreach(Floor* floor, floors){
        if( floor->id() == floorId){
            floor->setVisible(true);
            setCurrentFloor(floor);
            found = true;
        }else{
            floor->setVisible(false);
        }
    }
    if(!found){ //如果没找到对应的defaultFloor
        Floor *floor = floors.at(0);
        floor->setVisible(true);
        setCurrentFloor(floor);
    }
    update();

    return true;
}

bool Scene::showDefaultFloor(){
    return showFloor(1);
}

void Scene::setCurrentFloor(Floor *floor){
    m_curFloor = floor;
}

Floor* Scene::currentFloor() const {
    return m_curFloor;
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    ToolManager::instance()->currentTool().contextMenuEvent(event);
}

QList<Feature *> Scene::findMapFeature(const QString &name){
    //TODO: better stategies
    return m_root->findChildren<Feature*>(name);
}

QList<QList<Feature*> > Scene::findAllRepeat(){
    QList<QList<Feature*> > result;
    QMultiHash<QString, Feature* > hash;
    if(m_building != NULL){
        const QVector<Floor*> & floors = m_building->getFloors();
        Floor* floor;
        foreach(floor, floors){
            Q_ASSERT_X(floor != NULL, "findAllRepeat", "floor is NULL");
            Room* room;
            const QList<Room *> & rooms = floor->getRooms();
            foreach (room, rooms) {
                QString name = room->objectName();
                if(name.compare("非开放区域") && name.compare("中空") && name.compare("空铺")){
                    // hash.insertMulti(room->objectName(), room);
                    hash.insert(room->objectName(), room);
                }

            }
        }
    }

    const QList<QString> & keys = hash.uniqueKeys();
    foreach (const QString & key, keys) {
        const QList<Feature*> & list = hash.values(key);
        if(list.size() > 1){
            result.append(list);
        }
    }

    return result;
}

void Scene::selectMapFeature(Feature *feature){
    if(feature != NULL && (feature->isClassOf("room") || feature->isClassOf("PubPoint"))){
        clearSelection();
        Floor *floor = static_cast<Floor*>(feature->parentObject());
        if(floor != NULL){
            showFloor(floor->id());
        }
        feature->setSelected(true);
    }
}

void Scene::transformMap(const QTransform &mat)
{
    if(m_curFloor != NULL){//rotate current floor
        m_curFloor->transformFeature(mat);
    }else{ //rotate the whole building
        m_building->transformFeature(mat);
    }
}

void Scene::addScale(double s) {
    m_scaleNum ++;
    double scale = (s*m_curScale + m_curScale*(m_scaleNum-1))/m_scaleNum; //相对于初始的平均缩放
    QTransform mat;
    scale /= m_curScale; //相对于上一次的缩放
    mat.scale(scale, scale);
    m_building->transformFeature(mat);
    m_curScale = scale;

}

void Scene::setSelectedLayer(Feature *feature){
    m_selectedLayers.clear();
    m_selectedLayers.append(feature);
}

void Scene::clearSelectedLayers(){
    m_selectedLayers.clear();
}

Feature *Scene::currentLayer(){
    if(m_selectedLayers.empty())
        return NULL;
    else
        return m_selectedLayers.back();
}
