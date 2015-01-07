#include "pubpointtool.h"
#include "../core/pubpoint.h"
#include "../core/building.h"
#include "../core/scene.h"
#include "../gui/documentview.h"
#include <QGraphicsSceneEvent>

PubPointTool::PubPointTool(DocumentView *doc) :
    AbstractTool(doc)
{
}

void PubPointTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        PubPoint *pubPoint = new PubPoint(m_doc->building());
        pubPoint->setCenter(event->scenePos().toPoint());
        pubPoint->setSelected(true);
        m_doc->scene()->addPubPoint(pubPoint);
        m_doc->scene()->clearSelection();
        m_doc->selectionChanged(pubPoint);
        pubPoint->update(pubPoint->boundingRect());
    }
}
