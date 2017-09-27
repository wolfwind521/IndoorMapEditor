#include "pubpoint.h"
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QJsonObject>
#include <QApplication>
#include "../gui/documentview.h"

#pragma execution_character_set("utf-8")
QHash<QString, int> PubPoint::m_typeHash;

PubPoint::PubPoint(QGraphicsItem *parent) :
    Feature(parent)
{
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);

    PubPoint::m_typeHash["未设置"] = 0;
    PubPoint::m_typeHash["出入口"] = 22006;
    PubPoint::m_typeHash["电梯"] = 21003;
    PubPoint::m_typeHash["扶梯"] = 21002;
    PubPoint::m_typeHash["楼梯"] = 21001;
    PubPoint::m_typeHash["柜台"] = 11003;
    PubPoint::m_typeHash["ATM"] = 11002;
    PubPoint::m_typeHash["问讯处"] = 11004;
    PubPoint::m_typeHash["卫生间"] = 11001;
    PubPoint::m_typeHash["休息处"] = 11008;
}

bool PubPoint::load(const QJsonObject &jsonObject)
{
    Feature::load(jsonObject);
    m_type = jsonObject["Type"].toString();
    m_id = jsonObject["_id"].toInt();

    QJsonArray point = jsonObject["Outline"].toArray()[0].toArray()[0].toArray();
    m_center = QPoint(point[0].toInt(), -point[1].toInt());

    if(m_type == "0" || m_type == ""){
        if(m_enName == "Gate"){
            m_type = "22006";
        }else if(m_enName == "Cashier"){
            m_type = "11003";
        }else if ( m_enName == "ATM"){
            m_type = "11002";
        }else if(m_enName == "Elevator"){
            m_type = "21003";
        }else if(m_enName == "Escalator"){
            m_type = "21002";
        }else if(m_enName == "Info"){
            m_type = "11004";
        }else if(m_enName == "Toilet" || m_enName == "AccessibleToilet"){
            m_type = "11001";
        }else if(m_enName == "Stair"){
            m_type = "21001";
        }else if(m_enName == "RestRoom"){
            m_type = "11008";
        }
    }
    return true;
}

bool PubPoint::save(QJsonObject &jsonObject) const
{
    Feature::save(jsonObject);
    jsonObject["Type"] = m_type;
    jsonObject["_id"] = m_id;

    QJsonArray point;
    point.append(int(m_center.x()));
    point.append(int(-m_center.y()));

    QJsonArray array0, array1;
    array0.append(point);
    array1.append(array0);

    jsonObject["Outline"] = array1;
    return true;
}

QRectF PubPoint::boundingRect() const
{
    const int padding = 15;
    QRectF rect(m_center, m_center);
    rect.adjust(-padding, -padding, padding, padding);
    return rect;
}

QPainterPath PubPoint::shape() const
{
    QPainterPath path;
    path.addEllipse(m_center, 15, 15);
    return path;
}

void PubPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if(DocumentView::viewStyle() & DocumentView::StyleShowPointText){

        //if selected
        QColor color(125, 125, 125);
        QColor fillColor = (option->state & QStyle::State_Selected) ? color.darker(150) : color;
        bool highlight = ((option->state & QStyle::State_Selected) || (option->state & QStyle::State_MouseOver) ) ;
        QColor borderColor = highlight ? QColor(0, 160, 233) : color.darker();
        QPen borderPen(borderColor, highlight ? 5 : 1);
        painter->setBrush(fillColor);
        painter->setPen(borderPen);
        painter->drawEllipse(m_center, 15, 15);

//        painter->setPen(QPen());
//        QFont font = QApplication::font("DocumentView");
//        font.setPixelSize(font.pointSize());
//        //font.setPixelSize(12);
//        painter->setFont(font);
//        QRect fontRect = QFontMetrics(font).boundingRect(objectName());
//        int width = fontRect.width();
//        int height = fontRect.height();
//        painter->drawText(QPoint(m_center.x()-width/2.0, m_center.y() - height/2.0), objectName());

        QPixmap pixmap;
        if(m_type == "22006")
            pixmap = QPixmap(":src/icon/entry.png");
        else if(m_type == "11002")
            pixmap = QPixmap(":src/icon/ATM.png");
        else if(m_type == "21003")
            pixmap = QPixmap(":src/icon/lift.png");
        else if(m_type == "21002")
            pixmap = QPixmap(":src/icon/escalator.png");
        else if(m_type == "21001")
            pixmap = QPixmap(":src/icon/stair.png");
        else if(m_type == "11001")
            pixmap = QPixmap(":src/icon/toilet.png");
        painter->drawPixmap(m_center.x()-15.0, m_center.y()-15.0, pixmap);
    }
}

const QStringList PubPoint::typeStringList() const{
    QStringList typeList;
    typeList<<"未设置"<<"出入口"<<"电梯"<<"扶梯"<<"楼梯"<<"柜台"<<"ATM"<<"问讯处"<<"卫生间"<<"休息处";
    return typeList;
}

QString PubPoint::getTypeName(){
    return PubPoint::m_typeHash.key(m_type.toInt());
}

void PubPoint::updateByTypeName(const QString &typeName){
    int value = PubPoint::m_typeHash[typeName];
    setType(QString::number(value));
}
