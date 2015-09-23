#include "selecttool.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/mapentity.h"
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#pragma execution_character_set("utf-8")

SelectTool::SelectTool(DocumentView *doc) :
    AbstractTool(doc)
{
}

void SelectTool::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    Scene *scene = m_doc->scene();
    if(scene->selectedItems().size() > 0){ //if sth selected
        MapEntity* entity = static_cast<MapEntity*>(scene->selectedItems().at(0));
        if(entity != NULL && entity->inherits("PolygonEntity")){ //if polygon entity selected
            if(m_contextMenu == NULL){ //create the menu
                m_contextMenu = new QMenu();
                QAction *toBuildingAction = m_contextMenu->addAction("设为建筑");
                QAction *toFloorAction = m_contextMenu->addAction("设为楼层");
                QAction *toFuncAreaAction = m_contextMenu->addAction("设为房间");

                connect(toBuildingAction, SIGNAL(triggered()), scene, SLOT(convertSelectedToBuilding()));
                connect(toFloorAction, SIGNAL(triggered()), scene, SLOT(convertSelectedToFloor()));
                connect(toFuncAreaAction, SIGNAL(triggered()), scene, SLOT(convertSelectedToFuncArea()));
            }
            m_contextMenu->exec(event->screenPos());
        }
    }
}
