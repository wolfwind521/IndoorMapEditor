#ifndef PROPVIEWBUILDING_H
#define PROPVIEWBUILDING_H

#include "propertyview.h"

class MapEntity;
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QFormLayout)

class PropViewBuilding : public PropertyView
{
    Q_OBJECT
public:
    explicit PropViewBuilding(QWidget *parent = 0);
    ~PropViewBuilding();

    virtual bool match(const MapEntity * mapEntity) const;
    virtual void setMapEntity(MapEntity * mapEntity);
public slots:
    void updateHeight(const QString &height);
    void updateDefaultFloor(const QString &floorid);
private:
    QLabel *m_underFloorsLabel;
    QLabel *m_groundFloorsLabel;

    QLineEdit *m_heightEdit;
    QLineEdit *m_defaultFloorEdit;
};

#endif // PROPVIEWBUILDING_H
