#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "abstracttool.h"

class SelectTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit SelectTool(DocumentView *doc);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // SELECTTOOL_H
