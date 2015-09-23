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
    MapEntity *curLayer = currentLayer();
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
                return addFuncArea(new FuncArea(floor));
            }
        }
        else{
            return addFuncArea(new FuncArea(m_curFloor));
        }
    }
}

void Scene::deletePolygonByContext(PolygonEntity *entity) {
    if(entity->isClassOf("Floor")){
        m_building->deleteFloor(static_cast<Floor*>(entity));
    }else{
        delete entity;
        entity = NULL;
    }
}

FuncArea* Scene::addFuncArea(FuncArea *funcArea){
    if(m_curFloor != NULL){
        funcArea->setParentEntity(m_curFloor);
        emit buildingChanged();
        return funcArea;
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
    imageLayer->setParentEntity(floor);
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
}

void Scene::convertSelectedToFuncArea(){
    PolygonEntity* selectedEntity = static_cast<PolygonEntity*>(selectedItems().at(0));
    addFuncArea(new FuncArea(*selectedEntity));
    selectedEntity->outline().clear();
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
        MapEntity *mapEntity = dynamic_cast<MapEntity *>(item);
        if(mapEntity)
            deleteMapEntity(mapEntity);
    }
    update();
}

void Scene::deleteSelectedLayers(){
    bool isBuilding = false;
    foreach(QGraphicsItem* item, m_selectedLayers){
        if(item){
            MapEntity *mapEntity = qgraphicsitem_cast<MapEntity*>(item);
            if(mapEntity->isClassOf("Building"))
                isBuilding = true;
            removeMapEntity(mapEntity);
            delete item;
            item = NULL;
        }
    }
    if(isBuilding){
        reset();
    }
    clearSelectedLayers();
}

void Scene::deleteMapEntity(MapEntity *entity){

    //如果是floor或building，仅清空其轮廓
    if(entity->isClassOf("Floor")||entity->isClassOf("Building")){
        PolygonEntity* poly = dynamic_cast<PolygonEntity*>(entity);
        poly->outline().clear();
    }else{ //否则直接删除
        removeMapEntity(entity);
        delete entity;
        entity = NULL;
    }
    emit buildingChanged();
}

void Scene::removeMapEntity(MapEntity *entity){
    entity->setParent(NULL);
    entity->setParentItem(NULL);
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

QList<MapEntity *> Scene::findMapEntity(const QString &name){
    //TODO: better stategies
    return m_root->findChildren<MapEntity*>(name);
}

QList<QList<MapEntity*> > Scene::findAllRepeat(){
    QList<QList<MapEntity*> > result;
    QHash<QString, MapEntity* > hash;
    if(m_building != NULL){
        const QVector<Floor*> & floors = m_building->getFloors();
        Floor* floor;
        foreach(floor, floors){
            Q_ASSERT_X(floor != NULL, "findAllRepeat", "floor is NULL");
            FuncArea* funcArea;
            const QList<FuncArea *> & funcAreas = floor->getFuncAreas();
            foreach (funcArea, funcAreas) {
                QString name = funcArea->objectName();
                if(name.compare("非开放区域") && name.compare("中空") && name.compare("空铺")){
                    hash.insertMulti(funcArea->objectName(), funcArea);
                }

            }
        }
    }

    const QList<QString> & keys = hash.uniqueKeys();
    foreach (const QString & key, keys) {
        const QList<MapEntity*> & list = hash.values(key);
        if(list.size() > 1){
            result.append(list);
        }
    }

    return result;
}

void Scene::selectMapEntity(MapEntity *entity){
    if(entity != NULL && (entity->isClassOf("FuncArea") || entity->isClassOf("PubPoint"))){
        clearSelection();
        Floor *floor = static_cast<Floor*>(entity->parentObject());
        if(floor != NULL){
            showFloor(floor->id());
        }
        entity->setSelected(true);
    }
}

void Scene::transformMap(const QMatrix &mat)
{
    if(m_curFloor != NULL){//rotate current floor
        m_curFloor->transformEntity(mat);
    }else{ //rotate the whole building
        m_building->transformEntity(mat);
    }
}

void Scene::addScale(double s) {
    m_scaleNum ++;
    double scale = (s*m_curScale + m_curScale*(m_scaleNum-1))/m_scaleNum; //相对于初始的平均缩放
    QMatrix mat;
    scale /= m_curScale; //相对于上一次的缩放
    mat.scale(scale, scale);
    m_building->transformEntity(mat);
    m_curScale = scale;

}

void Scene::setSelectedLayer(MapEntity *entity){
    m_selectedLayers.clear();
    m_selectedLayers.append(entity);
}

void Scene::clearSelectedLayers(){
    m_selectedLayers.clear();
}

MapEntity *Scene::currentLayer(){
    if(m_selectedLayers.empty())
        return NULL;
    else
        return m_selectedLayers.back();
}
