#ifndef PUBPOINT_H
#define PUBPOINT_H

#include "mapentity.h"

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

    PUB_TYPE pubType() const;
    void setPubType(PUB_TYPE type);

signals:
    void pubTypeChanged(PUB_TYPE type);

private:
    PUB_TYPE m_type;

};

#endif // PUBPOINT_H
