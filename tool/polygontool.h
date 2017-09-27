#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include "abstracttool.h"

class PolygonFeature;

class PolygonTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit PolygonTool(DocumentView* doc);

    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    bool m_start;
    bool m_isCreating;
    PolygonFeature* m_polygon;
};

#endif // POLYGONTOOL_H
