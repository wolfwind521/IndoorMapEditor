#include "polygonfeature.h"
#include "../math/gdiam.hpp"
#include "../gui/documentview.h"
#include <cmath>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

PolygonFeature::PolygonFeature(QGraphicsItem *parent)
    :Feature(parent), m_area(0.0), m_frontAngle(0)
{
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

PolygonFeature::PolygonFeature(const QString & name, QGraphicsItem *parent)
    : Feature(parent), m_area(0.0), m_frontAngle(0)
{
    setObjectName(name);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

PolygonFeature::PolygonFeature(const QString & name, int id)
    :  m_area(0.0), m_frontAngle(0)
{
    m_id = id;
    setObjectName(name);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

PolygonFeature::PolygonFeature(const QString & name, const QPolygon& poly)
    :m_outline(poly), m_frontAngle(0)
{
    setObjectName(name);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
    computeArea();
}


void PolygonFeature::copy(PolygonFeature &polygon)
{
    setObjectName(polygon.objectName());
    m_enName = polygon.enName();
    m_id = polygon.id();
    m_center = polygon.center();
    m_area = polygon.area();
    setOutline(polygon.outline());
}

void PolygonFeature::setOutline(const QVector<QPoint> & points)
{
   m_outline = QPolygon(points);
   update();
}

QPolygon & PolygonFeature::outline()
{
    return m_outline;
}

bool PolygonFeature::load(const QJsonObject &jsonObject)
{

    Feature::load(jsonObject);
    m_area = jsonObject["Area"].toDouble();

    const QJsonArray & jsonArray = jsonObject["Outline"].toArray()[0].toArray()[0].toArray();
    for(int i = 0; i < jsonArray.size() - 1; i+=2){
        m_outline.append(QPoint(jsonArray[i].toInt(), -jsonArray[i+1].toInt()));
    }

    if(m_area == 0){
        computeArea();
    }

    computeCenter(); //for diandao maps, the center is not right. so must recompute

    return true;
}

bool PolygonFeature::save(QJsonObject &jsonObject) const
{
    Feature::save(jsonObject);
    jsonObject["Area"] = int(m_area);
    QJsonArray jsonArray;
    for(int i = 0; i < m_outline.size(); i++){
        jsonArray.append(int(m_outline[i].x()));
        jsonArray.append(int(-m_outline[i].y()));
    }
    QJsonArray array0,array1;
    array0.append(jsonArray);
    array1.append(array0);

    jsonObject["Outline"] = array1;
    return true;
}

double PolygonFeature::area(){
    if(m_area < 0){
        computeArea();
    }
    return m_area;
}

void PolygonFeature::setArea(const double area)
{
    m_area = area;
}

void PolygonFeature::addPoint(const QPoint & p)
{
    m_outline.append(p);
}

void PolygonFeature::movePoint(const int id, const QPoint & vector)
{
    Q_ASSERT(id >= 0 && id < m_outline.size());
    m_outline[id] += vector;
}

void PolygonFeature::movePointTo(const int id, const QPoint & point){
    Q_ASSERT(id >= 0 && id < m_outline.size());
    m_outline[id] = point;
}

void PolygonFeature::insertPoint(const int id, const QPoint &p)
{
    m_outline.insert(id, p);
}

void PolygonFeature::removePoint(const int id)
{
    m_outline.removeAt(id);
}

int PolygonFeature::PointNum() const
{
    return m_outline.size();
}

QRectF PolygonFeature::boundingRect() const
{
    const int margin = 5;
    return m_outline.boundingRect().adjusted(-margin, -margin, margin, margin);
}

QPainterPath PolygonFeature::shape() const
{
    QPainterPath path;
    path.addPolygon(m_outline);
    return path;
}

void PolygonFeature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    //if selected
    QColor fillColor = (option->state & QStyle::State_Selected) ? m_color.darker(150) : m_color;

    QColor borderColor = ((option->state & QStyle::State_Selected) || (option->state & QStyle::State_MouseOver) ) ? QColor(0, 160, 233) : m_color.darker(250);

    //setZValue(100);

    painter->setBrush(fillColor);
    painter->setPen(QPen(borderColor, 1));
    painter->drawPolygon(m_outline);

    if(DocumentView::viewStyle() & DocumentView::StyleShowDirection && m_frontAngle != 0){
        painter->setPen(QPen(QColor(255, 0, 0), 2));
        painter->drawLine(0,0,2000,2000*tan(m_frontAngle));
    }
}

double PolygonFeature::computeArea()
{
    //Area = 1/2 \sum_{i}(x_{i}y_{i+1} - x_{i+1}y_{i})
    //refer to: http://www.efg2.com/Lab/Graphics/PolygonArea.htm
    if(m_outline.size() < 3){
        m_area = 0;
    }else{
        double sum = 0;
        QPoint p0, p1;
        for(int i = 0; i < m_outline.size()-1; i++){
            p0 = m_outline.at(i);
            p1 = m_outline.at(i+1);
            sum += p0.x() * p1.y() - p1.x() * p0.y();
        }

        //the last point
        p0 = m_outline.at(m_outline.size()-1);
        p1 = m_outline.at(0);
        sum += p0.x() * p1.y() - p1.x() * p0.y();

        m_area = fabs(0.5 * sum) / 100.0; //from dm to m
    }
    return m_area;
}

const QPointF & PolygonFeature::computeCenter(){
    QPointF point(0,0);
    int count = m_outline.size();
    for(int i = 0; i < count; i++){
        point += m_outline[i];
    }
    point /= qreal(count);
    m_center = point;

    emit centerChanged(m_center);
    return m_center;
}

QPointF PolygonFeature::computeMainDir(){
    gdiam_real *points;
    int num = m_outline.size();
    points = (gdiam_point)malloc( sizeof( gdiam_point_t ) * num );
    for(int i=0; i<num; i++){
        points[i*3] = m_outline[i].x();
        points[i*3 + 1] = m_outline[i].y();
        points[i*3 +2 ] = 0;
    }
    gdiam_point *pnt_arr = gdiam_convert((gdiam_real *)points, num);

    gdiam_bbox obb = gdiam_approx_mvbb_grid_sample(pnt_arr, num, 5, 400);
    obb.dump();

    free(points);
    free(pnt_arr);


    return QPointF(double(obb.get_dir(0)[0]), double(obb.get_dir(0)[1]));
}

void PolygonFeature::transformFeature(const QTransform &matrix){
    Feature::transformFeature(matrix);
    m_outline = matrix.map(m_outline);
    computeArea();
}

void PolygonFeature::setColor(QColor color){
    if(m_color == color)
        return;
    m_color = color;
}
