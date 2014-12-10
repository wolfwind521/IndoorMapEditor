#include "documentview.h"
#include "../core/building.h"
#include "../core/mapentity.h"
#include "../core/floor.h"
#include "../core/funcarea.h"
#include "../core/pubpoint.h"
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QUndoStack>


DocumentView::DocumentView():m_isModified(false), m_root(NULL), m_building(NULL)
{
    m_scene = new QGraphicsScene();
    m_undoStack = new QUndoStack(this);
    m_root = new MapEntity(tr("root"));
    m_scene->addItem(static_cast<QGraphicsItem*>(m_root));
    //m_building = new Building(tr("未命名"), m_root);
    this->setScene(m_scene);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::Antialiasing);

    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

}

DocumentView::~DocumentView()
{
}

MapEntity *DocumentView::root() const{
    return m_root;
}

Building* DocumentView::building() const{
    return m_building;
}

void DocumentView::setBuilding(Building *building)
{
    if(m_building == building)
        return;
    //delete m_building; //delete the old one

    m_building = building;
    m_building->setParentItem(m_root);
    m_building->setParent(m_root);

    emit buildingChanged(*m_building);
}

bool DocumentView::isModified()
{
    return m_isModified;
}

void DocumentView::setModified(bool b)
{
    m_isModified = b;
}

void DocumentView::clear()
{
    m_scene->clear();
    m_undoStack->clear();
}

void DocumentView::printScene(QPainter *painter){
    m_scene->render(painter);
}

void DocumentView::updateSelection(){
    if(m_scene->selectedItems().size() > 0){
        MapEntity* selectedEntity = static_cast<MapEntity*>(m_scene->selectedItems().at(0));
        emit selectionChanged(selectedEntity);
    }else{
        emit selectionChanged(NULL);
    }
}

void DocumentView::updateSelection(const QModelIndex & index){
    MapEntity* mapEntity = static_cast<MapEntity*>(index.internalPointer());

    //a floor selected, change the visible floor
    QString className = mapEntity->metaObject()->className();
    if( !className.compare( "Floor" )){
        QObject* floor;
        foreach (floor, m_building->children()) {
            static_cast<MapEntity*>(floor)->setVisible(false);
        }
        mapEntity->setVisible(true);
        this->update(this->contentsRect());
    }
    //a funcArea or pubPoint selected, change the visible floor
    else if( !className.compare("FuncArea") || !className.compare("PubPoint")){
        MapEntity* parent = static_cast<MapEntity*>(mapEntity->parent());
        if(!parent->isSelected()){
            QObject* floor;
            foreach (floor, m_building->children()) {
                static_cast<MapEntity*>(floor)->setVisible(false);
            }
            parent->setVisible(true);
        }
        m_scene->clearSelection();
    }

    mapEntity->setSelected(true);
}

QGraphicsScene * DocumentView::scene() const{
    return m_scene;
}
