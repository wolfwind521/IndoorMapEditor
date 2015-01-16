#include "polygontool.h"
#include "../gui/documentview.h"
#include "../core/funcarea.h"
#include "../core/building.h"
#include "../core/scene.h"
#include <QGraphicsSceneEvent>

PolygonTool::PolygonTool(DocumentView *doc) :
    AbstractTool(doc), m_start(true), m_isCreating(false), m_polygon(NULL)
{
}

void PolygonTool::mousePressEvent(QGraphicsSceneMouseEvent *event){

}

void PolygonTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton) {//left button down
        if(m_start){ //start creating
            m_polygon = m_doc->scene()->createPolygonByContext();
            m_start = false;
            m_isCreating = true;
            m_doc->scene()->clearSelection();

            m_polygon->addPoint(event->scenePos().toPoint());
        }
        m_polygon->addPoint(event->scenePos().toPoint());

        //redraw
        m_polygon->update(m_polygon->boundingRect());
    }else if(event->button() == Qt::RightButton){//right button down, finish
        //remove the last point
        m_polygon->removePoint(m_polygon->PointNum() - 1); //pop the end point

        if(m_polygon->PointNum() < 3){
            m_doc->scene()->deletePolygonByContext(m_polygon);
        }else{
            m_polygon->computeCenter();
            m_polygon->computeArea();

            //selection
            m_doc->scene()->clearSelection();
            m_polygon->setSelected(true);
            m_doc->selectionChanged(m_polygon);
        }
        m_start = true;
        m_isCreating = false;
        //redraw
        m_doc->scene()->update();
    }
}

void PolygonTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(m_isCreating){
        m_doc->scene()->update();
        m_polygon->movePointTo(m_polygon->PointNum()-1, event->scenePos().toPoint());
    }
}
