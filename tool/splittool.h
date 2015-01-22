#ifndef SPLITTOOL_H
#define SPLITTOOL_H

#include "abstracttool.h"
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QGraphicsItem>

//the split line for UI
class SplitLine: public QGraphicsItem
{
public:
    SplitLine();
    ~SplitLine();

    QPolygon &line();
    //QGraphicsItem functions
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPolygon* m_line;
};

//structure of the intersection point
struct CrossNode{
    QPoint crossPoint; //intersection point
    int polyPointId1; //edge point1 of the polygon
    int polyPointId2; //edge point2 of the polygon
    int linePointId1; //edge point1 of the line
    int linePointId2; //edge point2 of the line
};

//class of splitting a polygon by a polyline
class Split {
public:
    QVector<QPolygon> split( QPolygon &polygon, QPolygon &line);

private:
    bool intersect(QPointF &crossPoint, const QPoint &polyPoint1, const QPoint &polyPoint2, const QPoint &linePoint1, const QPoint &linePoint2);
    bool isBetween(const QPoint &p0, const QPoint &p1, const QPoint &p2); //if p0 is between p1 and p2
    QPolygon *m_polygon;
    QPolygon *m_polyLine;
    QVector<CrossNode> m_crossNodes;
};

class SplitTool : public AbstractTool
{

public:
    SplitTool(DocumentView *parent);
    ~SplitTool();

    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    virtual void keyPressEvent(QKeyEvent *event) {};
    virtual void keyReleaseEvent(QKeyEvent *event) {};

private:

    SplitLine *m_splitLine;
    bool m_start;
    bool m_isCreating;

};

#endif // SPLITTOOL_H
