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
    int dianpingId() const;
    void setDianpingId(int dpId);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:
    void updateName(const QString &name);
    void updateCenter(const QPointF &center);
private:
    QGraphicsTextItem *m_textItem;
    QString m_shopNo;
    int     m_dianpingId; //temp
};

#endif // FUNCAREA_H
