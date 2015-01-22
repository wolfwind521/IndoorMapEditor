#include "abstracttool.h"
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

AbstractTool::AbstractTool(DocumentView *doc)
    :m_doc(doc), m_contextMenu(NULL)
{
}

AbstractTool::~AbstractTool(){
    delete m_contextMenu;
}

bool AbstractTool::isClassOf(const QString &className) const {
    QString myClassName;
    myClassName = this->metaObject()->className();
    return (myClassName == className);
}
