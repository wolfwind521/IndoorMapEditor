#include "selecttool.h"
#include "../gui/documentview.h"
#include <QGraphicsSceneMouseEvent>

SelectTool::SelectTool(DocumentView *doc) :
    AbstractTool(doc)
{
}

void SelectTool::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ){
    event->accept();
}
