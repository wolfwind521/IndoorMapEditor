#ifndef FUNCAREA_H
#define FUNCAREA_H

#include "polygonentity.h"

#include <QString>
#include <QJsonObject>

enum FUNC_TYPE
{
    UNKOWN = -1,
    CHINESE_FOOD,
    //TODO: more types
};

class QGraphicsTextItem;

class FuncArea : public PolygonEntity
{

    Q_OBJECT

public:
    FuncArea(QGraphicsItem *parent = 0);
    FuncArea(PolygonEntity &polygon);
    FuncArea( const QString & name, const QPolygon& poly);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject, double scale) const;

    //setters and getters
    QString shopNo() const;
    void setShopNo(const QString & shopNo);
    int category() const;
    void setCategory(int cate);
    int dianpingId() const;
    void setDianpingId(int dpId);
    void setMateId(int id);
    int mateId() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private slots:
    void updateName(const QString &name);
    void updateCenter(const QPointF &center);
    void updateFont(const QFont &font);
private:
    QGraphicsTextItem *m_textItem;
    QString m_shopNo;
    int m_category;
    int     m_dianpingId; //temp
    bool m_connected; //slots have been connected
    int m_mateId;     //if this id is not 0, it is the same funcarea with another one
};

#endif // FUNCAREA_H
