#include "scene.h"
#include "../tool/toolmanager.h"
#include "../tool/abstracttool.h"
#include "mapentity.h"
#include "building.h"
#include "floor.h"
#include "imagelayer.h"
#include "funcarea.h"
#include "pubpoint.h"
#include <math.h>
#include <QList>
#include <QGraphicsSceneMouseEvent>

#pragma execution_character_set("utf-8")

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent), m_selectable(true), m_root(NULL), m_building(NULL), m_curFloor(NULL)
{
    createRoot();
}

void Scene::reset(){
    QGraphicsScene::clear();
    Floor::resetMaxFloorId();
    m_building = NULL;
    createRoot();
    setBuilding(new Building(tr("未命名建筑")));
    update();
}

MapEntity *Scene::root() const{
    return m_root;
}

void Scene::createRoot(){
    m_root = new MapEntity(tr("root")); //add a root node
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

//void Scene::addEntityByContext(PolygonEntity *polygon){
//    if(m_building->floorNum() == 0 || m_curFloor == NULL)
//    {
//        addFloor(new Floor(*polygon));
//    }else{
//        addFuncArea(new FuncArea(*polygon));
//    }
//    deleteMapEntity(polygon);
//}

PolygonEntity* Scene::createPolygonByContext() {
    PolygonEntity *entity;
    if(m_building->children().empty() || m_curFloor == NULL){
        Floor * floor = new Floor(m_building);
        m_building->addFloor(floor);
        m_curFloor = floor;
        entity = floor;
    }else{
        entity = new FuncArea(m_curFloor);
        entity->setParent(m_curFloor);
    }
    emit buildingChanged();
    return entity;
}

void Scene::deletePolygonByContext(PolygonEntity *entity) {
    if(entity->isClassOf("Floor")){
        m_building->deleteFloor(static_cast<Floor*>(entity));
    }else{
        delete entity;
        entity = NULL;
    }
}

void Scene::addFuncArea(FuncArea *funcArea){
    if(m_curFloor != NULL){
        funcArea->setParentEntity(m_curFloor);
    }else{
        funcArea->setParentEntity(m_building);
    }
    emit buildingChanged();
}

void Scene::addFloor(Floor *floor){
    if(m_building != NULL){
        m_building->addFloor(floor);
    }
    emit buildingChanged();
}

void Scene::addPubPoint(PubPoint *pubPoint){
    if(m_curFloor != NULL){
        pubPoint->setParentEntity(m_curFloor);
    }else{
        pubPoint->setParentEntity(m_building);
    }
    emit buildingChanged();
}

void Scene::addImageLayer(ImageLayer *imageLayer) {
//    if(m_curFloor != NULL) {
//        imageLayer->setParentEntity(m_curFloor);
//    }else{

//    }
    Floor *floor = new Floor();
    addFloor(floor);
    imageLayer->setParentEntity(floor);
    m_curFloor = floor;
    emit buildingChanged();
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

void Scene::setSelectable(bool b){
    m_selectable = b;
}

void Scene::convertSelectedToBuilding(){
    PolygonEntity* selectedEntity = static_cast<PolygonEntity*>(selectedItems().at(0));
    m_building->copy(*selectedEntity);
    deleteMapEntity(selectedEntity);
    emit buildingChanged();
}

void Scene::convertSelectedToFloor(){
    PolygonEntity* selectedEntity = static_cast<PolygonEntity*>(selectedItems().at(0));
    if(m_curFloor != NULL && m_curFloor->outline().empty()){
        m_curFloor->setOutline(selectedEntity->outline());
    }else{
        addFloor(new Floor(*selectedEntity));
    }
     deleteMapEntity(selectedEntity);
    emit buildingChanged();
}

void Scene::convertSelectedToFuncArea(){
    PolygonEntity* selectedEntity = static_cast<PolygonEntity*>(selectedItems().at(0));
    addFuncArea(new FuncArea(*selectedEntity));
    deleteMapEntity(selectedEntity);
    emit buildingChanged();
}

void Scene::deleteSelected(){
    QList<QGraphicsItem*> items = selectedItems();
    foreach (QGraphicsItem* item, items) {
        item->setSelected(false);
        deleteMapEntity(static_cast<MapEntity*>(item));
    }
//    emit selectionChanged();
}

void Scene::deleteMapEntity(MapEntity *entity){
    QString className = entity->metaObject()->className();
    if(className == "Floor"){
        m_building->deleteFloor(static_cast<Floor*>(entity));
    }else{
        removeMapEntity(entity);
        delete entity;
        entity = NULL;
    }
}

void Scene::removeMapEntity(MapEntity *entity){
    entity->setParent(NULL);
    entity->setParentItem(NULL);
    if(entity == m_curFloor){
        m_curFloor = NULL;
    }
    emit buildingChanged();
}

bool Scene::showFloor(int floorId) {
    QObject *object;
    bool found = false;
    foreach(object, building()->children()){
        Floor *floor = static_cast<Floor*>(object);
        if(floor->id() == floorId){
            floor->setVisible(true);
            setCurrentFloor(floor);
            found = true;
        }else{
            floor->setVisible(false);
        }
    }
    update();

    return found;
}

bool Scene::showDefaultFloor(){
    return showFloor(m_building->defaultFloor());
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

