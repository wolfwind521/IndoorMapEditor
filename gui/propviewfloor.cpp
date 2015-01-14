#include "propviewfloor.h"
#include "../core/floor.h"
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

PropViewFloor::PropViewFloor(QWidget *parent) :
    PropertyView(parent)
{
    m_heightEdit = new QLineEdit;
    m_funcAreaNumLabel = new QLabel;
    m_pubPointNumLabel = new QLabel;

    m_layout->addRow(tr("height"), m_heightEdit);
    m_layout->addRow(tr("Number of Shops "), m_funcAreaNumLabel);
    m_layout->addRow(tr("Number of PubPoints"), m_pubPointNumLabel);

    connect(m_heightEdit, SIGNAL(textEdited(QString)), this, SLOT(updateHeight(QString)));
}

bool PropViewFloor::match(const MapEntity *mapEntity) const {
    return mapEntity->isClassOf("Floor");
}

void PropViewFloor::setMapEntity(MapEntity *mapEntity) {
    if(m_mapEntity != mapEntity){
        PropertyView::setMapEntity(mapEntity);
        Floor *floor = static_cast<Floor*>(mapEntity);
        m_heightEdit->setText(QString::number(floor->height()));
        //m_funcAreaNum->setText(QString::number(floor->));
    }
}

void PropViewFloor::updateHeight(const QString &height) {
    static_cast<Floor*>(m_mapEntity)->setHeight(height.toDouble());
}
