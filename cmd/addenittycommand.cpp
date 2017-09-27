#include "addenittycommand.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/feature.h"

AddEnittyCommand::AddEnittyCommand(DocumentView *doc, Feature *feature, QUndoCommand *parent) :
    QUndoCommand(parent), m_doc(doc), m_feature(feature)
{
}

void AddEnittyCommand::undo(){
    m_doc->scene()->removeMapFeature(m_feature);
}

void AddEnittyCommand::redo(){

}
