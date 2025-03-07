#ifndef POLYGONENTITY_H
#define POLYGONENTITY_H

#include "feature.h"
#include <QString>
#include <QJsonArray>
#include <QGraphicsObject>
#include <QPolygon>
class PolygonFeature : public Feature
{
    Q_OBJECT
public:
    PolygonFeature(QGraphicsItem *parent = 0);
    PolygonFeature(const QString & name, QGraphicsItem *parent = 0);
    PolygonFeature( const QString & name, int id);
    PolygonFeature( const QString & name, const QPolygon& poly);

    //copy the data from @polygon, used in subclasses
    void copy(PolygonFeature &polygon);
    //setters and getters
    QPolygon & outline();
    void setOutline(const QVector<QPoint> & points);
    double area();
    void setArea(const double area);

    //add a point to tail
    void addPoint(const QPoint & p);

    //move the point at @id by @vector
    void movePoint(const int id, const QPoint & vector);

    //move the point at @id to @point
    void movePointTo(const int id, const QPoint & point);

    //insert a @point at @id
    void insertPoint(const int id, const QPoint & p);

    //remove the point at @id
    void removePoint(const int id);

    int PointNum() const;

    //set color
    void setColor(QColor color);

    //compute the center
    const QPointF & computeCenter();

    //compute the area
    double computeArea();

    //compute the OBB (oriented bounding box) main direction
    QPointF computeMainDir();

    //QGraphicsItem functions
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //io
    virtual bool load(const QJsonObject & jsonObject);
    virtual bool save(QJsonObject & jsonObject) const;

    virtual void transformFeature(const QTransform &matrix);
protected:

    double m_frontAngle;

    QPolygon m_outline;
    QColor m_color;
    double m_area;
};

#endif // POLYGONENTITY_H
