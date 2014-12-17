#include "scene.h"
#include "../tool/toolmanager.h"
#include "../tool/abstracttool.h"
#include "mapentity.h"
#include "building.h"
#include "floor.h"
#include "funcarea.h"
#include "pubpoint.h"
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent), m_selectable(true), m_root(NULL), m_building(NULL), m_polygonContextMenu(NULL), m_curFloor(NULL)
{
    createRoot();
}

void Scene::reset(){
    QGraphicsScene::clear();
    createRoot();
    setBuilding(new Building(tr("未命名")));
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

PolygonEntity* Scene::createPolygonByContext(){
    PolygonEntity *entity;
    if(m_building->children().empty() || m_curFloor == NULL){
        entity = new Floor(m_building);
        entity->setParent(m_building);
        m_curFloor = static_cast<Floor*>(entity);
    }else{
        entity = new FuncArea(m_curFloor);
        entity->setParent(m_curFloor);
    }
    emit buildingChanged();
    return entity;
}

void Scene::addFuncArea(FuncArea *funcArea){
    if(m_curFloor != NULL){
        funcArea->setParent(m_curFloor);
        funcArea->setParentItem(m_curFloor);
    }else{
        funcArea->setParent(m_building);
        funcArea->setParentItem(m_building);
    }
    emit buildingChanged();
}

void Scene::addFloor(Floor *floor){
    if(m_building != NULL){
        floor->setParent(m_building);
        floor->setParentItem(m_building);
    }
    emit buildingChanged();
}

void Scene::addPubPoint(PubPoint *pubPoint){
    if(m_curFloor != NULL){
        pubPoint->setParent(m_curFloor);
        pubPoint->setParentItem(m_curFloor);
    }else{
        pubPoint->setParent(m_building);
        pubPoint->setParentItem(m_building);
    }
    emit buildingChanged();
}

void Scene::mousePressEvent( QGraphicsSceneMouseEvent *event ){
    if(m_selectable)
        QGraphicsScene::mousePressEvent(event);
    ToolManager::instance()->currentTool().mousePressEvent(event);
}

void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ){
    if(m_selectable)
        QGraphicsScene::mouseReleaseEvent(event);
    ToolManager::instance()->currentTool().mouseReleaseEvent(event);
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
    addFloor(new Floor(*selectedEntity));
    deleteMapEntity(selectedEntity);
    emit buildingChanged();
}

void Scene::convertSelectedToFuncArea(){
    PolygonEntity* selectedEntity = static_cast<PolygonEntity*>(selectedItems().at(0));
    addFuncArea(new FuncArea(*selectedEntity));
    deleteMapEntity(selectedEntity);
    emit buildingChanged();
}

void Scene::deleteMapEntity(MapEntity *entity){
    entity->setParent(NULL);
    entity->setParentItem(NULL);
    delete entity;
    entity = NULL;
}

void Scene::setCurrentFloor(Floor *floor){
    m_curFloor = floor;
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    if(selectedItems().size() > 0){ //if sth selected
        if(static_cast<MapEntity*>(selectedItems().at(0))->inherits("PolygonEntity")){ //if polygon entity selected
            if(m_polygonContextMenu == NULL){ //create the menu
                m_polygonContextMenu = new QMenu();
                QAction *toBuildingAction = m_polygonContextMenu->addAction("设为建筑");
                QAction *toFloorAction = m_polygonContextMenu->addAction("设为楼层");
                QAction *toFuncAreaAction = m_polygonContextMenu->addAction("设为房间");

                connect(toBuildingAction, SIGNAL(triggered()), this, SLOT(convertSelectedToBuilding()));
                connect(toFloorAction, SIGNAL(triggered()), this, SLOT(convertSelectedToFloor()));
                connect(toFuncAreaAction, SIGNAL(triggered()), this, SLOT(convertSelectedToFuncArea()));
            }
            m_polygonContextMenu->exec(event->screenPos());
        }
    }
}

