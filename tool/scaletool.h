#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "abstracttool.h"
#include <QPointF>

QT_FORWARD_DECLARE_CLASS(QGraphicsLineItem)

class ScaleTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit ScaleTool(DocumentView *doc);
    ~ScaleTool();

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    bool m_start;
    bool m_isCreating;
    QGraphicsLineItem *m_scaleLine; //for GUI
    QPointF m_firstPoint;
};

#endif // SCALETOOL_H
