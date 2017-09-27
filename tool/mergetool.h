#ifndef MERGETOOL_H
#define MERGETOOL_H

#include "abstracttool.h"

class PolygonFeature;
class Room;
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
    QPolygon merge(QVector<QPolygon>& polygons);
    QList<QGraphicsItem*> m_selectedList;
};

#endif // MERGETOOL_H
