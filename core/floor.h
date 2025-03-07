#ifndef FLOOR_H
#define FLOOR_H

#include "polygonfeature.h"
#include <QJsonObject>

class Room;

class Floor : public PolygonFeature
{
    Q_OBJECT

    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Floor(QGraphicsItem *parent = 0);
    Floor(PolygonFeature & polygon);
    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject & jsonObject) const;

    QList<Room*> getRooms() const;

    //setters and getters
    double height() const;
    void setHeight(double height);
    static void resetMaxFloorId();

    //transform
    virtual void transformFeature(const QTransform &matrix);
    virtual int generateId();
signals:
    void heightChanged( double height );
private:
    double m_height;
    static int m_maxFloorId;
};

#endif // FLOOR_H
