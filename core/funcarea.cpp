#include "funcarea.h"

#include "../gui/documentview.h"
#include <QPainter>
#include <QApplication>
#include <QGraphicsTextItem>


#pragma execution_character_set("utf-8")


QHash<QString, int> FuncArea::m_typeHash;

FuncArea::FuncArea(QGraphicsItem *parent)
    : PolygonEntity(parent), m_dianpingId(-1), m_connected(false), m_mateId(0)
{
    m_color = QColor(248, 203, 173, 150);
    setObjectName(tr("未命名"));

    FuncArea::m_typeHash["未设置"] = 0;
    FuncArea::m_typeHash["中空区域"] = 100;
    FuncArea::m_typeHash["封闭区域"] = 300;
    FuncArea::m_typeHash["空铺"] = 400;
    FuncArea::m_typeHash["餐饮"] = 101;
    FuncArea::m_typeHash["购物"] = 102;
    FuncArea::m_typeHash["美妆丽人"] = 103;
    FuncArea::m_typeHash["亲子儿童"] = 104;
    FuncArea::m_typeHash["生活服务"] = 105;
    FuncArea::m_typeHash["教育培训"] = 106;
    FuncArea::m_typeHash["生活方式"] = 107;
    FuncArea::m_typeHash["休闲娱乐"] = 108;
    FuncArea::m_typeHash["其他"] = 109;

    m_textItem = new QGraphicsTextItem(this);
    m_textItem->setPos(center());
    m_textItem->setPlainText(objectName());
    m_textItem->setFlag(QGraphicsItem::ItemIsMovable);
    //m_textItem->setFlag(QGraphicsItem::ItemIsSelectable);
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

void FuncArea::setCategory( int cate){
    if(m_category == cate)
        return;
    m_category = cate;
}

int FuncArea::category() const{
    return m_category;
}

int FuncArea::dianpingId() const {
    return m_dianpingId;
}

void FuncArea::setDianpingId(int dpId) {
    if(m_dianpingId == dpId)
        return;

    m_dianpingId = dpId;
}

void FuncArea::setMateId(int id){
    if(m_mateId == id)
        return;
    m_mateId = id;
}

int FuncArea::mateId() const {
    return m_mateId;
}

bool FuncArea::load(const QJsonObject &jsonObject) {
    PolygonEntity::load(jsonObject);

    m_type = jsonObject["Type"].toString();
    m_category = jsonObject["Category"].toInt();
    m_id = jsonObject["_id"].toInt();
    m_shopNo = jsonObject["ShopNo"].toString();
    m_dianpingId = jsonObject["dianping_id"].toInt();
    m_mateId = jsonObject["MateId"].toInt();
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
    jsonObject["MateId"] = m_mateId;
    jsonObject["Category"] = m_category;

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

QVariant FuncArea::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == ItemVisibleHasChanged){
        if(scene() && !m_connected){
            m_textItem->setFont(scene()->font());
            connect(scene(), SIGNAL(fontChanged(QFont)), this, SLOT(updateFont(QFont)) );
            m_connected = true;
            return 0;
        }
    }else{
        return QGraphicsItem::itemChange(change, value);
    }
}

void FuncArea::updateFont(const QFont &font){
    m_textItem->setFont(font);
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

            //m_textItem->setFont(scene()->font());
            //m_textItem->setZValue(1000.0);
            m_textItem->show();
        }else{
            m_textItem->hide();
        }
    }
}

const QStringList FuncArea::typeStringList() const{
    QStringList typeList;
    typeList<<"未设置"<<"中空区域"<<"封闭区域"<<"空铺"<<"餐饮"<<"购物"<<"美妆丽人"<<"亲子儿童"<<"生活服务"<<"教育培训"<<"生活方式"<<"休闲娱乐"<<"其他";
    return typeList;
}

QString FuncArea::getTypeName(){

    if(m_category == 0){//if category is undefined
        int type = m_type.toInt();
        return FuncArea::m_typeHash.key(type);
    }else{
        return FuncArea::m_typeHash.key(m_category);
    }
}
void FuncArea::updateByTypeName(const QString &typeName){
    int value = FuncArea::m_typeHash[typeName];
    if(value == 100 || value == 300 || value == 400){
        setCategory(0);
        setType(QString::number(value));
//        setObjectName(typeName);
//        setEnName(QString());
    }else{
        setCategory(value);
    }
}
