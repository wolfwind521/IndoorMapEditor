#include "room.h"

#include "../gui/documentview.h"
#include <QPainter>
#include <QApplication>
#include <QGraphicsTextItem>


#pragma execution_character_set("utf-8")


QHash<QString, int> Room::m_typeHash;

Room::Room(QGraphicsItem *parent)
    : PolygonFeature(parent), m_dianpingId(-1), m_connected(false), m_mateId(0)
{
    m_color = QColor(248, 203, 173, 150);
    setObjectName(tr(""));
    m_type = "400";
    m_category = Category(109);
    m_areaStatus = Working;
    m_sortType = UNSORTED;

    Room::m_typeHash["未设置"] = 0;
    Room::m_typeHash["中空区域"] = 100;
    Room::m_typeHash["封闭区域"] = 300;
    Room::m_typeHash["空铺"] = 400;
    Room::m_typeHash["餐饮"] = 101;
    Room::m_typeHash["购物"] = 102;
    Room::m_typeHash["美妆丽人"] = 103;
    Room::m_typeHash["亲子儿童"] = 104;
    Room::m_typeHash["生活服务"] = 105;
    Room::m_typeHash["教育培训"] = 106;
    Room::m_typeHash["生活方式"] = 107;
    Room::m_typeHash["休闲娱乐"] = 108;
    Room::m_typeHash["其他"] = 109;

    m_textItem = new QGraphicsTextItem(this);
    m_textItem->setPos(center());
    m_textItem->setPlainText(objectName());
    m_textItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_textItem->setFlag(QGraphicsItem::ItemIsSelectable);
    connect(this, SIGNAL(objectNameChanged(QString)), this, SLOT(updateName(QString)));
    connect(this, SIGNAL(centerChanged(QPointF)), this, SLOT(updateCenter(QPointF)) );
}

Room::Room(PolygonFeature &polygon)
{
    new (this) Room();
    copy(polygon);
    m_dianpingId = -1;

    m_textItem->setPlainText(objectName());
    m_textItem->setPos(center());
}

Room::Room( const QString & name, const QPolygon& poly)
{
    new (this) Room();
    m_outline = poly;
    m_dianpingId = -1;
    m_textItem->setPos(center());
}

QString Room::shopNo() const {
    return m_shopNo;
}

void Room::setShopNo(const QString &shopNo) {
    if(m_shopNo == shopNo)
        return;
    m_shopNo = shopNo;
}

void Room::setCategory( Room::Category cate){
    if(m_category == cate)
        return;
    m_category = cate;
}

Room::Category Room::category() const{
    return m_category;
}

int Room::dianpingId() const {
    return m_dianpingId;
}

void Room::setDianpingId(int dpId) {
    if(m_dianpingId == dpId)
        return;

    m_dianpingId = dpId;
}

void Room::setMateId(int id){
    if(m_mateId == id)
        return;
    m_mateId = id;
}

int Room::mateId() const {
    return m_mateId;
}

void Room::setAreaStatus(Room::AreaStatus status){
    if(m_areaStatus == status)
        return;
    m_areaStatus = status;
}

Room::AreaStatus Room::areaStatus() const{
    return m_areaStatus;
}

bool Room::load(const QJsonObject &jsonObject) {
    PolygonFeature::load(jsonObject);

    m_type = jsonObject["Type"].toString();
    m_category = Category(jsonObject["Category"].toInt());
    m_id = jsonObject["_id"].toInt();
    if(m_id == 0){
        generateId();
    }
    m_shopNo = jsonObject["ShopNo"].toString();
    m_dianpingId = jsonObject["dianping_id"].toInt();
    m_mateId = jsonObject["MateId"].toInt();
    m_areaStatus = AreaStatus(jsonObject["AreaStatus"].toInt(int(Working)));
    m_brandShop = jsonObject["BrandShop"].toInt(-1);
    m_sortType = SORT_TYPE(jsonObject["SortType"].toInt(int(UNSORTED)));
    if(m_dianpingId < 0 && m_dianpingId != -1){
        m_dianpingId = -1;
    }
    if(m_type == "-1" || m_type.size()>6){
        m_type = "0";
    }
    m_textItem->setPos(center());
    m_textItem->setPlainText(objectName());
    updateColor();

    return true;
}

bool Room::save(QJsonObject &jsonObject) const {
    PolygonFeature::save(jsonObject);

    jsonObject["Type"] = m_type;
    jsonObject["_id"] = m_id;
    jsonObject["BrandShop"] = m_brandShop;
    jsonObject["ShopNo"] = m_shopNo;
    jsonObject["dianping_id"] = m_dianpingId;
    jsonObject["MateId"] = m_mateId;
    jsonObject["Category"] = int(m_category);
    jsonObject["SortType"] = int(m_sortType);
    jsonObject["AreaStatus"] = int(m_areaStatus);

    QJsonArray jsonArray;
    jsonArray.append(int(m_center.x()));
    jsonArray.append(int(-m_center.y()));
    jsonObject["Center"] = jsonArray;
    return true;
}

void Room::updateName(const QString &name){
    m_textItem->setPlainText(name);
}

void Room::updateCenter(const QPointF &center){
    m_textItem->setPos(center);
}

QVariant Room::itemChange(GraphicsItemChange change, const QVariant &value){
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

void Room::updateFont(const QFont &font){
    m_textItem->setFont(font);
}

void Room::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    PolygonFeature::paint(painter, option, widget);

    //paint the marker
    if(!m_center.isNull()){
        //setZValue(1);
        painter->setBrush(QColor(22, 22, 22));
        painter->setPen(QPen(QColor(22, 22, 22)));
        painter->drawEllipse(m_center, 3, 3);

        (DocumentView::viewStyle() & DocumentView::StyleShowShopName) ? m_textItem->show() : m_textItem->hide();

    }
}

const QStringList Room::typeStringList() const{
    QStringList typeList;
    typeList<<"未设置"<<"中空区域"<<"封闭区域"<<"空铺"<<"餐饮"<<"购物"<<"美妆丽人"<<"亲子儿童"<<"生活服务"<<"教育培训"<<"生活方式"<<"休闲娱乐"<<"其他";
    return typeList;
}

QString Room::getTypeName(){

    if(m_category == 0){//if category is undefined
        int type = m_type.toInt();
        return Room::m_typeHash.key(type);
    }else{
        return Room::m_typeHash.key(m_category);
    }
}
void Room::updateByTypeName(const QString &typeName){
    int value = Room::m_typeHash[typeName];
    if(value == 100 || value == 300 || value == 400){
        setCategory(None); //将业态置空
        setType(QString::number(value)); //更新type
    }else{
        int tmp = m_type.toInt();
        if(tmp == 100 || tmp == 300 || tmp == 400){
            setType(QString::number(value)); //将type设为其他值
        }
        setCategory(Category(value));//更新业态
    }
}

void Room::setSortType(Room::SORT_TYPE sortType){
    if(m_sortType == sortType)
        return;
    m_sortType = sortType;
    updateColor();

}

Room::SORT_TYPE Room::sortType(){
    return m_sortType;
}

void Room::updateColor(){
    if(m_sortType == MIDDLE_AREA){
        m_color = QColor(255, 0, 0, 100);
    }else if(m_sortType == SIDE_AREA){
        m_color = QColor(0, 0, 255, 100);
    }else{
        m_color = QColor(248, 203, 173, 150);
    }
}
