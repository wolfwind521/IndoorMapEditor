#include "splittool.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include "../core/room.h"
#include "../core/floor.h"
#include <QPoint>
#include <QPolygon>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

//------------SplitLine methods-----------------
SplitLine::SplitLine(){
    m_line = new QPolygon();
}

SplitLine::~SplitLine(){
    delete m_line;
}

QPolygon & SplitLine::line() {
    return *m_line;
}

QRectF SplitLine::boundingRect() const {
    return m_line->boundingRect();
}

QPainterPath SplitLine::shape() const {
    QPainterPath path;
    path.addPolygon(*m_line);
    return path;
}

void SplitLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    QColor color = QColor(195, 13, 35);
    painter->setPen(QPen(color, 1));
    painter->drawPolyline(*m_line);
}

//---------------Split methods------------------

QVector<QPolygon> Split::split( QPolygon &polygon, QPolygon &line){
    m_polygon = &polygon;
    m_polyLine = &line;
    m_crossNodes.clear();

    //if is valid
    QVector<QPolygon> polygons;
    if(polygon.size() < 3 || line.size() < 2)
        return polygons;

    //get all the intersection nodes
    QPointF crossPoint;
    int i, j;
    for(i=0; i<line.size()-1; i++){
        for (j=0; j<polygon.size()-1; j++){
            if(intersect(crossPoint, polygon[j], polygon[j+1], line[i], line[i+1])){
                CrossNode node = {crossPoint.toPoint(), j, j+1, i, i+1};
                m_crossNodes.push_back(node);
            }
        }
        //the last edge of the polygon
        if(intersect(crossPoint, polygon[j], polygon[0], line[i], line[i+1])){
            CrossNode node = {crossPoint.toPoint(), j, 0, i, i+1};
            m_crossNodes.push_back(node);
        }
    }

    if(m_crossNodes.size() != 2){
        QMessageBox::warning(0, "","暂不支持非两个交点的情况",QMessageBox::Ok);
        return polygons;
    }

    QPolygon poly1, poly2;
    //add the first intersection point
    poly1.append(m_crossNodes[0].crossPoint);
    poly2.append(m_crossNodes[0].crossPoint);

    //use the points along the line
    for(i = m_crossNodes[0].linePointId2; i<line.size(); i++){
        if(i != m_crossNodes[1].linePointId2){
            poly1.append(line[i]);
            poly2.append(line[i]);
            if(i == m_crossNodes[1].linePointId1){
                break;
            }
        }else{
            break;
        }
    }

    //add the second intersection point
    poly1.append(m_crossNodes[1].crossPoint);
    poly2.append(m_crossNodes[1].crossPoint);

    //if the tow intersection points are on the same edge
    bool crossSameEdge = false;
    if(m_crossNodes[1].polyPointId1 == m_crossNodes[0].polyPointId1)
    {
        crossSameEdge = true;
    }

    i = m_crossNodes[1].polyPointId1;
    if(!(crossSameEdge &&
            isBetween(m_crossNodes[0].crossPoint, polygon[i], m_crossNodes[1].crossPoint)))
    {
         //get the points along one side of the polygon
        for( ; i >= 0; i--){
            poly1.append(polygon[i]);
            if(i == m_crossNodes[0].polyPointId2){
                break;
            }
        }
        if(i<0){
            for(i = polygon.size()-1; i>=0; i--){
                poly1.append(polygon[i]);
                if(i == m_crossNodes[0].polyPointId2){
                    break;
                }
            }
        }
    }


    //get the points along the other side of the polygon
    i = m_crossNodes[1].polyPointId2;
    if(!(crossSameEdge &&
        isBetween(m_crossNodes[0].crossPoint, polygon[i], m_crossNodes[1].crossPoint))){

        for(; i < polygon.size(); i++){
            poly2.append(polygon[i]);
            if(i == m_crossNodes[0].polyPointId1){
                break;
            }
        }
        if(i == polygon.size()){
            for(i = 0; i < polygon.size(); i++){
                poly2.append(polygon[i]);
                if(i == m_crossNodes[0].polyPointId1){
                    break;
                }
            }
        }
    }
    polygons.append(poly1);
    polygons.append(poly2);
    return polygons;
}

bool Split::intersect(QPointF &crossPoint, const QPoint &polyPoint1, const QPoint &polyPoint2, const QPoint &linePoint1, const QPoint &linePoint2) {
    QLineF l1(polyPoint1, polyPoint2);
    QLineF l2(linePoint1, linePoint2);

    return l1.intersects(l2, &crossPoint) == QLineF::BoundedIntersection;
}

bool Split::isBetween(const QPoint &p0, const QPoint &p1, const QPoint &p2) {
    QPoint vec1 = p1 - p0;
    QPoint vec2 = p0 - p2;

    return (vec1.x()*vec2.x()) >= 0 && (vec1.y()*vec2.y())>=0;
}

//---------------SplitTool methods--------------
SplitTool::SplitTool(DocumentView *parent)
    :AbstractTool(parent), m_splitLine(NULL), m_start(true), m_isCreating(false)
{
}

SplitTool::~SplitTool(){
    delete m_splitLine;
    m_splitLine = NULL;
}

void SplitTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Scene *scene = m_doc->scene();

    //if nothing or more than one thing selected
    if(scene->selectedItems().size() != 1){
        QMessageBox::warning(m_doc, tr(""),tr("请先选中一个店铺"),QMessageBox::Ok);
        return;
    }

    //if the selected item is not a Room
    QGraphicsItem *item = scene->selectedItems().at(0);
    PolygonFeature* poly = dynamic_cast<PolygonFeature*>(item);
    if(poly == NULL || !poly->isClassOf("Room")){
        QMessageBox::warning(m_doc, tr(""),tr("请先选中一个店铺"),QMessageBox::Ok);
        return;
    }

    Room *room = static_cast<Room*>(poly);
    if(event->button() == Qt::LeftButton) {
        if(m_start){
            m_splitLine = new SplitLine();
            scene->addItem(m_splitLine);
            m_start = false;
            m_isCreating = true;
            m_splitLine->line().append(event->scenePos().toPoint());
        }
        m_splitLine->line().append(event->scenePos().toPoint());
        m_splitLine->update();
    }else if(event->button() == Qt::RightButton) {
        if(m_isCreating){
            Split split;
            QVector<QPolygon> polygons = split.split(room->outline(), m_splitLine->line());
            if(polygons.isEmpty()){

            }else{
                delete room; //delete the old one
                foreach(QPolygon poly, polygons){
                    room = new Room("未命名", poly);
                    room->computeArea();
                    room->computeCenter();
                    room->setParentFeature(scene->currentFloor());
                }
            }
        }

        delete m_splitLine;
        m_start = true;
        m_isCreating = false;

        m_doc->scene()->update();
    }
}

void SplitTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(m_isCreating && m_splitLine != NULL){
        m_doc->scene()->update();
        m_splitLine->line().last() = event->scenePos().toPoint();
    }
}
