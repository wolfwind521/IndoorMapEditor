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

    Q_PROPERTY(PUB_TYPE pubType READ pubType WRITE setPubType NOTIFY pubTypeChanged)

public:
    explicit PubPoint(QGraphicsItem *parent = 0);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject, double scale) const;

    PUB_TYPE pubType() const;
    void setPubType(PUB_TYPE type);

    //QGraphicsItem functions
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void pubTypeChanged(PUB_TYPE type);

private:
    PUB_TYPE m_type;

};

#endif // PUBPOINT_H
