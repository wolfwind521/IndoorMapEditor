/***************************************************
 ** Building
 ** @brief: the model for building data
 ** @author: gaimeng
 ** @date: Nov, 2014
 **
 ****************************************************/
#ifndef BUILDING_H
#define BUILDING_H

#include "polygonentity.h"

#include <QString>
#include <QGraphicsItem>

class Floor;

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
    Building(PolygonEntity &polygon);
    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject & jsonObject) const;
    int floorNum() const;
    int underFloors() const;
    int groundFloors() const;
    int defaultFloor() const;
    void setDefaultFloor(int floorId);
    double height() const;
    void setHeight(double height);

    void addFloor(Floor * floor);
    void deleteFloor(Floor * floor);

    QVector<Floor*> getFloors();

 private slots:
    void updateFloorIds(int oldId, int newId);
private:
    int m_underFloors;
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
