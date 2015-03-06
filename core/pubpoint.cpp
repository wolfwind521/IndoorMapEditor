#include "pubpoint.h"
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QJsonObject>
#include <QApplication>
#include "../gui/documentview.h"

PubPoint::PubPoint(QGraphicsItem *parent) :
    MapEntity(parent)
{
    setFlags(ItemIsSelectable);
}

bool PubPoint::load(const QJsonObject &jsonObject)
{
    MapEntity::load(jsonObject);
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

bool PubPoint::save(QJsonObject &jsonObject, double scale) const
{
    MapEntity::save(jsonObject, scale);
    jsonObject["Type"] = m_type;
    jsonObject["_id"] = m_id;

    QJsonArray point;
    point.append(int(m_center.x() * scale));
    point.append(int(-m_center.y() *scale));

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
    //    //if mouse over
    //    if (option->state & QStyle::State_MouseOver)
    //        fillColor = fillColor.lighter(125);

        painter->setBrush(fillColor);
        painter->setPen(QPen(fillColor.darker(),1));
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
