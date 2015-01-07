#include "addenittycommand.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/mapentity.h"

AddEnittyCommand::AddEnittyCommand(DocumentView *doc, MapEntity *entity, QUndoCommand *parent) :
    QUndoCommand(parent), m_doc(doc), m_entity(entity)
{
}

void AddEnittyCommand::undo(){
    m_doc->scene()->removeMapEntity(m_entity);
}

void AddEnittyCommand::redo(){

}
