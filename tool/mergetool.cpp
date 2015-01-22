#include "mergetool.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/polygonentity.h"
#include "../core/funcarea.h"
#include "../core/floor.h"
#include <QMenu>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>

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
    QVector<PolygonEntity*> polyArray;
    foreach(QGraphicsItem* item, itemList) {
        PolygonEntity* poly = dynamic_cast<PolygonEntity*>(item);
        if(poly->isClassOf("FuncArea")){
            polyArray.push_back(poly);
        }
    }
    PolygonEntity *polygon = merge(polyArray);
    FuncArea *newArea = new FuncArea(*polygon);
    newArea->computeArea();
    newArea->computeCenter();
    newArea->setParentEntity(scene->currentFloor());
}

PolygonEntity* MergeTool::merge(QVector<PolygonEntity *> &polygons) {
    while(polygons.size() > 2){
        QVector<PolygonEntity *> twoPoly;
        twoPoly.push_back(polygons.last());
        polygons.pop_back();
        twoPoly.push_back(polygons.last());
        polygons.pop_back();
        PolygonEntity* tmpPoly = merge(twoPoly);
        polygons.push_back(tmpPoly);
    }

    if(polygons.size() == 2){
        const QPolygon& p1 = polygons[0]->outline();
        const QPolygon& p2 = polygons[1]->outline();
        PolygonEntity* newPoly = new PolygonEntity(QString("未命名"), p1.united(p2));
        delete polygons[0];
        delete polygons[1];
        return newPoly;
    }

}
