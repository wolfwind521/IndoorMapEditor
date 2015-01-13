#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include "abstracttool.h"

class PolygonEntity;

class PolygonTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit PolygonTool(DocumentView* doc);

    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event){}
    virtual void keyReleaseEvent(QKeyEvent *event){}
private:
    bool m_start;
    bool m_isCreating;
    PolygonEntity* m_polygon;
};

#endif // POLYGONTOOL_H
