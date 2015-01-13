#ifndef PROPVIEWFLOOR_H
#define PROPVIEWFLOOR_H

#include "propertyview.h"

class Floor;
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)


class PropViewFloor : public PropertyView
{
    Q_OBJECT
public:
    PropViewFloor(QWidget *parent = 0);
    virtual bool match(const MapEntity *mapEntity) const;
    virtual void setMapEntity(MapEntity * MapEntity);

public slots:
    void updateHeight(const QString &height);
private:
    QLineEdit * m_heightEdit;
    QLabel * m_funcAreaNumLabel;
    QLabel * m_pubPointNumLabel;

};

#endif // PROPVIEWFLOOR_H
