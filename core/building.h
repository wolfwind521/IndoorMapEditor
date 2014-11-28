#ifndef BUILDING_H
#define BUILDING_H

#include "polygonentity.h"

#include <QString>
#include <QGraphicsItem>

class Building : public PolygonEntity
{
    Q_OBJECT
public:
    enum BUILDING_TYPE
    {
        MALL = 6,
        STORE
        //TODO: maybe other types
    };
    Building(const QString & name, QGraphicsItem *parent = 0);
    bool load(const QJsonObject & jsonObject);

private:
    int m_underfloors;
    double m_frontAngle;
    int m_defaultFloor;
    double m_height;
    int m_groundFloors;
    QString m_postCode;
    QString m_remark;
    QString m_floorsId;
    double m_latitude;
    double m_longitude;
    int m_version;
    BUILDING_TYPE m_type;
    QString m_key;
};

#endif // BUILDING_H
