#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "abstracttool.h"

class SelectTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit SelectTool(DocumentView *doc);

    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ){};
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ){};
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event){};
    virtual void keyPressEvent(QKeyEvent *event) {};
    virtual void keyReleaseEvent(QKeyEvent *event) {};

};

#endif // SELECTTOOL_H
