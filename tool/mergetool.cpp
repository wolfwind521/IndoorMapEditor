#include "mergetool.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/polygonfeature.h"
#include "../core/room.h"
#include "../core/floor.h"
#include <QMenu>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>

#pragma execution_character_set("utf-8")

MergeTool::MergeTool(DocumentView *parent) :
    AbstractTool(parent)
{
}

void MergeTool::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    event->setModifiers(Qt::ControlModifier);
}

void MergeTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
//    Scene *scene = m_doc->scene();
//    QGraphicsItem* curItem = scene->itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
//    if(curItem == NULL){
//        m_selectedList.clear();
//        scene->clearSelection();
//    }else{
//        foreach(QGraphicsItem* item, m_selectedList){
//            item->setSelected(true);
//        }
//        curItem->setSelected(!item->isSelected());
//        if(!curItem->isSelected()){
//            m_selectedList.removeOne(curItem);
//        }else{
//            m_selectedList.append(curItem);
//        }
//    }
    event->setModifiers(Qt::ControlModifier);
    //foreach(GraphicsItem* item, m_selectedList)
}

void MergeTool::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    Scene *scene = m_doc->scene();
    QList<QGraphicsItem*> itemList = scene->selectedItems();
    if(itemList.size() > 1) {
        if(m_contextMenu == NULL){
            m_contextMenu = new QMenu();
            QAction *mergeAction = m_contextMenu->addAction("合并");
            connect(mergeAction, SIGNAL(triggered()), this, SLOT(mergeSelectedItems()));
        }
        m_contextMenu->exec(event->screenPos());
    }
}

void MergeTool::mergeSelectedItems() {
    Scene *scene = m_doc->scene();
    QList<QGraphicsItem*> itemList = scene->selectedItems();
    QVector<QPolygon> polyArray;
    Floor *tmpFloor = NULL;

    //get all the polygons
    foreach(QGraphicsItem* item, itemList) {
        PolygonFeature* poly = dynamic_cast<PolygonFeature*>(item);
        if(poly != NULL){
            polyArray.push_back(poly->outline());
            if(poly->isClassOf("Floor")){
                tmpFloor = dynamic_cast<Floor*>(poly);
            }
        }
    }

    //merge the polygons
    if(polyArray.size() >= 2){
        QPolygon polygon = merge(polyArray);
        if(tmpFloor){
            tmpFloor->setOutline(polygon);
        }else{
            Room *newArea = new Room("未命名", polygon);
            newArea->computeArea();
            newArea->computeCenter();
            newArea->setParentFeature(scene->currentFloor());
        }
    }

    //delte the old funcAreas
    foreach(QGraphicsItem* item, itemList) {
        Room* room = dynamic_cast<Room*>(item);
        if(room != NULL){
            delete room;
        }
    }
    scene->clearSelection();
}

QPolygon MergeTool::merge(QVector<QPolygon > &polygons) {
    while(polygons.size() > 2){
        QVector<QPolygon > twoPoly;
        twoPoly.push_back(polygons.last());
        polygons.pop_back();
        twoPoly.push_back(polygons.last());
        polygons.pop_back();
        QPolygon tmpPoly = merge(twoPoly);
        polygons.push_back(tmpPoly);
    }

    if(polygons.size() == 2){
        return polygons[0].united(polygons[1]);
    }

}
