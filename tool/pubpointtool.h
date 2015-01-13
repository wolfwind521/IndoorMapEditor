#ifndef PUBPOINTTOOL_H
#define PUBPOINTTOOL_H

#include "abstracttool.h"

class PubPointTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit PubPointTool(DocumentView *doc);

    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void keyPressEvent(QKeyEvent *event){};
    virtual void keyReleaseEvent(QKeyEvent *event){} ;
};

#endif // PUBPOINTTOOL_H
