#include "funcarea.h"

#include "../gui/documentview.h"
#include <QPainter>
#include <QApplication>
#include <QGraphicsTextItem>


#pragma execution_character_set("utf-8")

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent), m_dianpingId(-1)
{
    m_color = QColor(248, 203, 173, 150);
    setObjectName(tr("未命名"));

    m_textItem = new QGraphicsTextItem(this);
    m_textItem->setPos(center());
    m_textItem->setPlainText(objectName());
    m_textItem->setFlag(QGraphicsItem::ItemIsMovable);

    connect(this, SIGNAL(objectNameChanged(QString)), this, SLOT(updateName(QString)));
    connect(this, SIGNAL(centerChanged(QPointF)), this, SLOT(updateCenter(QPointF)) );
}

FuncArea::FuncArea(PolygonEntity &polygon)
{
    new (this) FuncArea();
    copy(polygon);
    m_dianpingId = -1;

    m_textItem->setPlainText(objectName());
    m_textItem->setPos(center());
}

FuncArea::FuncArea( const QString & name, const QPolygon& poly)
{
    new (this) FuncArea();
    m_outline = poly;
    m_dianpingId = -1;
    m_textItem->setPos(center());
}

QString FuncArea::shopNo() const {
    return m_shopNo;
}

void FuncArea::setShopNo(const QString &shopNo) {
    if(m_shopNo == shopNo)
        return;
    m_shopNo = shopNo;
}

int FuncArea::dianpingId() const {
    return m_dianpingId;
}

void FuncArea::setDianpingId(int dpId) {
    if(m_dianpingId == dpId)
        return;

    m_dianpingId = dpId;
}

bool FuncArea::load(const QJsonObject &jsonObject) {
    PolygonEntity::load(jsonObject);

    m_type = jsonObject["Type"].toString();
    m_id = jsonObject["_id"].toInt();
    m_shopNo = jsonObject["ShopNo"].toString();
    m_dianpingId = jsonObject["dianping_id"].toInt();
    if(m_dianpingId < 0 && m_dianpingId != -1){
        m_dianpingId = -1;
    }
    if(m_type == "-1" || m_type.size()>6){
        m_type = "0";
    }
    m_textItem->setPos(center());
    m_textItem->setPlainText(objectName());

    return true;
}

bool FuncArea::save(QJsonObject &jsonObject, double scale) const {
    PolygonEntity::save(jsonObject, scale);

    jsonObject["Type"] = m_type;
    jsonObject["_id"] = m_id;
    jsonObject["ShopNo"] = m_shopNo;
    jsonObject["dianping_id"] = m_dianpingId;

    QJsonArray jsonArray;
    jsonArray.append(m_center.x() *scale);
    jsonArray.append(-m_center.y() *scale);
    jsonObject["Center"] = jsonArray;
    return true;
}

void FuncArea::updateName(const QString &name){
    m_textItem->setPlainText(name);
}

void FuncArea::updateCenter(const QPointF &center){
    m_textItem->setPos(center);
}

void FuncArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    PolygonEntity::paint(painter, option, widget);

    //paint the marker
    if(!m_center.isNull()){
        //setZValue(1);
        painter->setBrush(QColor(22, 22, 22));
        painter->setPen(QPen(QColor(22, 22, 22)));
        painter->drawEllipse(m_center, 3, 3);

        if(DocumentView::viewStyle() & DocumentView::StyleShowShopName){
//            //paint the text
//            painter->setPen(QPen());
//            QFont font = scene()->font();
//            font.setPixelSize(font.pointSize());
//            //font.setPixelSize(12);
//            painter->setFont(font);
//            QRect fontRect = QFontMetrics(font).boundingRect(objectName());
//            int width = fontRect.width();
//            int height = fontRect.height();
//            painter->drawText(QPoint(m_center.x()-width/2.0, m_center.y() - height/5.0), objectName());

            m_textItem->setFont(scene()->font());
            //m_textItem->setZValue(1000.0);
            m_textItem->show();
        }else{
            m_textItem->hide();
        }
    }
}
