#ifndef FLOOR_H
#define FLOOR_H

#include "polygonentity.h"
#include <QJsonObject>

class FuncArea;

class Floor : public PolygonEntity
{
    Q_OBJECT

    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Floor(QGraphicsItem *parent = 0);
    Floor(PolygonEntity & polygon);
    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject & jsonObject) const;

    QList<FuncArea*> getFuncAreas() const;

    //setters and getters
    double height() const;
    void setHeight(double height);
    static void resetMaxFloorId();

    //transform
    virtual void transformEntity(const QMatrix &matrix);
    virtual int generateId();
signals:
    void heightChanged( double height );
private:
    double m_height;
    static int m_maxFloorId;
};

#endif // FLOOR_H
