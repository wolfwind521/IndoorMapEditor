#include "documentview.h"
#include "../core/building.h"
#include "../core/mapentity.h"
#include "../core/floor.h"
#include "../core/funcarea.h"
#include "../core/pubpoint.h"
#include "../core/scene.h"
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QUndoStack>
#include <QtPrintSupport/QPrinter>


DocumentView::DocumentView()
    :m_isModified(false), m_selectable(true)
{
    m_scene = new Scene(this);
    m_scene->reset();
    m_undoStack = new QUndoStack(this);

    this->setScene(m_scene);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::Antialiasing);

    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

}

DocumentView::~DocumentView()
{
}

bool DocumentView::isModified()
{
    return m_isModified;
}

void DocumentView::setModified(bool b)
{
    m_isModified = b;
}

Building* DocumentView::building(){
    return m_scene->building();
}

void DocumentView::clear()
{
    m_scene->reset();
    m_undoStack->clear();
}

void DocumentView::printScene(QPrinter *printer){
    QPainter painter;
    painter.begin(printer);
    m_scene->render(&painter, printer->pageRect(), m_scene->sceneRect(), Qt::KeepAspectRatio);
    painter.end();
}

//selection from graphics view
void DocumentView::updateSelection(){
    if(m_scene->selectedItems().size() > 0){
        MapEntity* selectedEntity = static_cast<MapEntity*>(m_scene->selectedItems().at(0));
        emit selectionChanged(selectedEntity);
    }else{
        emit selectionChanged(NULL);
    }
}

//selection from tree view
void DocumentView::updateSelection(const QModelIndex & index){
    MapEntity *mapEntity = static_cast<MapEntity*>(index.internalPointer());

    //a floor selected, change the visible floor
    QString className = mapEntity->metaObject()->className();
    if( className == "Floor"){
        QObject *floor;
        foreach (floor, m_scene->building()->children()) {
            static_cast<MapEntity*>(floor)->setVisible(false);
        }
        mapEntity->setVisible(true);
        m_scene->setCurrentFloor(static_cast<Floor*>(mapEntity));
        this->update();
    }
    //a funcArea or pubPoint selected, change the visible floor
    else if( !className.compare("FuncArea") || !className.compare("PubPoint")){
        MapEntity* parent = static_cast<MapEntity*>(mapEntity->parent());
        if(!parent->isSelected()){
            QObject* floor;
            foreach (floor, m_scene->building()->children()) {
                static_cast<MapEntity*>(floor)->setVisible(false);
            }
            parent->setVisible(true);
            m_scene->setCurrentFloor(static_cast<Floor*>(parent));
        }
        m_scene->clearSelection();
    }
    else if(!className.compare("Building")){
        m_scene->setCurrentFloor(NULL);
    }

    mapEntity->setSelected(true);
    emit selectionChanged(mapEntity);
}

Scene * DocumentView::scene() const{
    return m_scene;
}

void DocumentView::setSelectable(bool b){
    if(m_selectable != b){
        m_selectable = b;
        if(m_selectable){
            this->setDragMode(QGraphicsView::RubberBandDrag);
        }else{
            this->setDragMode(QGraphicsView::NoDrag);
        }
        m_scene->setSelectable(m_selectable);
    }
}
