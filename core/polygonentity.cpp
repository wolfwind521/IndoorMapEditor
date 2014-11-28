#include "polygonentity.h"

#include <QtWidgets>

PolygonEntity::PolygonEntity(QGraphicsItem *parent)
    :MapEntity(parent), m_area(0.0)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

PolygonEntity::PolygonEntity(const QString & name, QGraphicsItem *parent)
    : MapEntity(parent), m_area(0.0)
{
    m_name = name;
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

PolygonEntity::PolygonEntity(const QString & name, int id) :  m_area(0.0)
{
    m_name = name;
    m_id = id;
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

PolygonEntity::PolygonEntity(const QString & name, int id, QPolygon poly, double area)
    :m_outline(poly), m_area(area)
{
    m_name = name;
    m_id = id;
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}



void PolygonEntity::setOutline(const QVector<QPoint> & points)
{
   m_outline = QPolygon(points);
}

void PolygonEntity::setOutline(const QJsonArray &jsonArray)
{
    for(int i = 0; i < jsonArray.size() - 1; i+=2){
        m_outline.append(QPoint(jsonArray[i].toInt(), jsonArray[i+1].toInt()));
    }
}

const QPolygon & PolygonEntity::outline() const
{
    return m_outline;
}

double PolygonEntity::area(){
    if(m_area < 0){
        computeArea();
    }
    return m_area;
}

void PolygonEntity::setArea(const double area)
{
    m_area = area;
}

void PolygonEntity::addPoint(const QPoint & p)
{
    m_outline.append(p);
}

void PolygonEntity::movePoint(const int id, const QPoint & vector)
{
    Q_ASSERT(id >= 0 && id < m_outline.size());
    m_outline[id] += vector;
}

void PolygonEntity::insertPoint(const int id, const QPoint &p)
{
    m_outline.insert(id, p);
}

void PolygonEntity::removePoint(const int id)
{
    m_outline.removeAt(id);
}

int PolygonEntity::PointNum() const
{
    return m_outline.size();
}

QRectF PolygonEntity::boundingRect() const
{
    const int margin = 1;
    return m_outline.boundingRect().adjusted(-margin, -margin, margin, margin);
}

QPainterPath PolygonEntity::shape() const
{
    QPainterPath path;
    path.addPolygon(m_outline);
    return path;
}

void PolygonEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    //if selected
    QColor fillColor = (option->state & QStyle::State_Selected) ? m_color.darker(150) : m_color;
    //if mouse over
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter(125);

    painter->setBrush(fillColor);
    painter->setPen(QPen(m_color.darker(),1));
    painter->drawPolygon(m_outline);
}

double PolygonEntity::computeArea()
{
    //TODO: compute the polygon area
    return m_area;
}

const QPointF & PolygonEntity::computeCenter(){
    QPointF point(0,0);
    int count = m_outline.size();
    for(int i = 0; i < count; i++){
        point += m_outline[i];
    }
    point /= qreal(count);
    m_center = point;
    return m_center;
    emit centerChanged(m_center);
}
