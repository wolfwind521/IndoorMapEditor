#include "funcarea.h"
#include "../gui/documentview.h"
#include <QPainter>
#include <QApplication>


#pragma execution_character_set("utf-8")

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent), m_dianpingId(-1)
{
    m_color = QColor(248, 203, 173, 150);
    setObjectName(tr("未命名"));
}

FuncArea::FuncArea(PolygonEntity &polygon)
{
    new (this) FuncArea();
    copy(polygon);
}

FuncArea::FuncArea( const QString & name, const QPolygon& poly)
{
    new (this) FuncArea();
    m_outline = poly;
}

FUNC_TYPE FuncArea::funcType() const {
    return m_type;
}

void FuncArea::setFuncType(const FUNC_TYPE type) {
    if(m_type == type)
        return;
    m_type = type;
    emit funcTypeChanged(m_type);
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

    m_type = static_cast<FUNC_TYPE>(jsonObject["Type"].toString().toInt());
    m_id = jsonObject["_id"].toInt();
    m_shopNo = jsonObject["ShopNo"].toString();
    m_dianpingId = jsonObject["dianping_id"].toInt();
    return true;
}

bool FuncArea::save(QJsonObject &jsonObject, double scale) const {
    PolygonEntity::save(jsonObject, scale);

    jsonObject["Type"] = QString::number(static_cast<int>(m_type));
    jsonObject["_id"] = m_id;
    jsonObject["ShopNo"] = m_shopNo;
    jsonObject["dianping_id"] = m_dianpingId;

    QJsonArray jsonArray;
    jsonArray.append(m_center.x() *scale);
    jsonArray.append(-m_center.y() *scale);
    jsonObject["Center"] = jsonArray;
    return true;
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
            //paint the text
            painter->setPen(QPen());
            QFont font = QApplication::font("DocumentView");
            font.setPixelSize(font.pointSize());
            //font.setPixelSize(12);
            painter->setFont(font);
            QRect fontRect = QFontMetrics(font).boundingRect(objectName());
            int width = fontRect.width();
            int height = fontRect.height();
            painter->drawText(QPoint(m_center.x()-width/2.0, m_center.y() - height/5.0), objectName());
        }
    }
}
