#ifndef FUNCAREA_H
#define FUNCAREA_H

#include "polygonentity.h"

#include <QString>
#include <QJsonObject>

enum FUNC_TYPE
{
    CHINESE_FOOD,
    //TODO: more types
};

class FuncArea : public PolygonEntity
{

    Q_OBJECT
    Q_PROPERTY(FUNC_TYPE funcType READ funcType WRITE setFuncType NOTIFY funcTypeChanged)
public:
    FuncArea(QGraphicsItem *parent = 0);
    FuncArea(PolygonEntity &polygon);
    FuncArea( const QString & name, const QPolygon& poly);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject) const;

    //setters and getters
    FUNC_TYPE funcType() const;
    void setFuncType(const FUNC_TYPE type);
    QString shopNo() const;
    void setShopNo(const QString & shopNo);
    int dianpingId() const;
    void setDianpingId(int dpId);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void funcTypeChanged(FUNC_TYPE type);
private:

    FUNC_TYPE m_type;
    QString m_shopNo;
    int     m_dianpingId; //temp
};

#endif // FUNCAREA_H
