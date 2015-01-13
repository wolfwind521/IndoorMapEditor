#include "propviewbuilding.h"
#include "../core/building.h"
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

PropViewBuilding::PropViewBuilding(QWidget *parent):
    PropertyView(parent)
{
    m_underFloorsLabel = new QLabel;
    m_groundFloorsLabel = new QLabel;
    m_heightEdit = new QLineEdit;
    m_defaultFloorEdit = new QLineEdit;
\
    m_layout->addRow(tr("高度"), m_heightEdit);
    m_layout->addRow(tr("默认楼层"), m_defaultFloorEdit);
    m_layout->addRow(tr("地下层数"), m_underFloorsLabel);
    m_layout->addRow(tr("地上层数"), m_groundFloorsLabel);

    connect(m_heightEdit, SIGNAL(textEdited(QString)), this, SLOT(updateHeight(QString)));
    connect(m_defaultFloorEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDefaultFloor(QString)));
}

PropViewBuilding::~PropViewBuilding() {

}

bool PropViewBuilding::match(const MapEntity *mapEntity) const {
    return mapEntity->isClassOf("Building");
}

void PropViewBuilding::setMapEntity(MapEntity *mapEntity) {
    if(mapEntity != m_mapEntity){
        PropertyView::setMapEntity(mapEntity);
        Building *building = static_cast<Building*>(mapEntity);
        m_heightEdit->setText(QString::number(building->height()));
        m_defaultFloorEdit->setText(QString::number(building->defaultFloor()));
        m_underFloorsLabel->setText(QString::number(building->underFloors()));
        m_groundFloorsLabel->setText(QString::number(building->groundFloors()));
    }
}

void PropViewBuilding::updateDefaultFloor(const QString &floorid) {
    static_cast<Building*>(m_mapEntity)->setDefaultFloor(floorid.toInt());
}

void PropViewBuilding::updateHeight(const QString &height) {
    static_cast<Building*>(m_mapEntity)->setHeight(height.toDouble());
}
