#include "pubpoint.h"

PubPoint::PubPoint(QGraphicsItem *parent) :
    MapEntity(parent)
{
}

PUB_TYPE PubPoint::pubType() const
{
    return m_type;
}

void PubPoint::setPubType(PUB_TYPE type)
{
    if(m_type == type)
        return;
    m_type = type;
    emit pubTypeChanged(m_type);
}
