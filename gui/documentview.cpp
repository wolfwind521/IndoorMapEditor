#include "documentview.h"
#include "core/building.h"
#include "core/mapentity.h"
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
