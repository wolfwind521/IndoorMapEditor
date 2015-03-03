#ifndef PUBPOINT_H
#define PUBPOINT_H

#include "mapentity.h"
#include <QString>
#include <QJsonArray>

enum PUB_TYPE{
    ATM = 11002,
    CASHIER = 11003,

};

class PubPoint : public MapEntity
{
    Q_OBJECT

public:
    explicit PubPoint(QGraphicsItem *parent = 0);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject, double scale) const;

    //QGraphicsItem functions
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PUBPOINT_H
