#ifndef MERGETOOL_H
#define MERGETOOL_H

#include "abstracttool.h"

class PolygonEntity;
class FuncArea;
class DocumentView;
class QGraphicsItem;

class MergeTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit MergeTool(DocumentView *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private slots:
    void mergeSelectedItems();
private:
    PolygonEntity* merge(QVector<PolygonEntity*>& polygons);
    QList<QGraphicsItem*> m_selectedList;
};

#endif // MERGETOOL_H
