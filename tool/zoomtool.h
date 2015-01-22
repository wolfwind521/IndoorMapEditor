#ifndef ZOOMTOOL_H
#define ZOOMTOOL_H

#include "abstracttool.h"

class ZoomTool : public AbstractTool
{
public:
    ZoomTool(DocumentView *parent);
    //TODO
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ){} ;
    virtual void keyPressEvent(QKeyEvent *event) {};
    virtual void keyReleaseEvent(QKeyEvent *event) {};

private:
    bool m_ctrlKeyDown;
};

#endif // ZOOMTOOL_H
