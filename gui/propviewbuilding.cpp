#include "propviewbuilding.h"
#include "../core/building.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QFormLayout>

#pragma execution_character_set("utf-8")

PropViewBuilding::PropViewBuilding(QWidget *parent):
    PropertyView(parent)
{
    m_underFloorsLabel = new QLabel;
    m_groundFloorsLabel = new QLabel;
    m_heightEdit = new QLineEdit;
    m_defaultFloorEdit = new QLineEdit;
    m_addressEdit = new QLineEdit;
    m_postCodeEdit = new QLineEdit;
    m_latitudeEdit = new QLineEdit;
    m_longitudeEdit = new QLineEdit;
    m_timeEdit = new QLineEdit;
    m_telEdit = new QLineEdit;
\
    m_layout->addRow(tr("高度"), m_heightEdit);
    m_layout->addRow(tr("默认楼层"), m_defaultFloorEdit);
    m_layout->addRow(tr("地下层数"), m_underFloorsLabel);
    m_layout->addRow(tr("地上层数"), m_groundFloorsLabel);
    m_layout->addRow(tr("地址"), m_addressEdit);
    m_layout->addRow(tr("邮编"), m_postCodeEdit);
    m_layout->addRow(tr("经度"), m_longitudeEdit);
    m_layout->addRow(tr("纬度"), m_latitudeEdit);
    m_layout->addRow(tr("营业时间"), m_timeEdit);
    m_layout->addRow(tr("电话"), m_telEdit);

    connect(m_heightEdit, SIGNAL(textEdited(QString)), this, SLOT(updateHeight(QString)));
    connect(m_defaultFloorEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDefaultFloor(QString)));
    connect(m_addressEdit, SIGNAL(textEdited(QString)), this, SLOT(updateAddress(QString)));
    connect(m_postCodeEdit, SIGNAL(textEdited(QString)), this, SLOT(updatePostCode(QString)));
    connect(m_longitudeEdit, SIGNAL(textEdited(QString)), this, SLOT(updateLongitudeEdit(QString)));
    connect(m_latitudeEdit, SIGNAL(textEdited(QString)), this, SLOT(updateLatitudeEdit(QString)));
    connect(m_timeEdit, SIGNAL(textEdited(QString)), this, SLOT(updateTimeEdit(QString)));
    connect(m_telEdit, SIGNAL(textEdited(QString)), this, SLOT(updateTelEdit(QString)));
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
        m_addressEdit->setText(building->address());
        m_postCodeEdit->setText(building->postCode());
        m_latitudeEdit->setText(QString::number(building->latitude()));
        m_longitudeEdit->setText(QString::number(building->longitude()));
        m_timeEdit->setText(building->time());
        m_telEdit->setText(building->tel());
    }
}

void PropViewBuilding::updateDefaultFloor(const QString &floorid) {
    static_cast<Building*>(m_mapEntity)->setDefaultFloor(floorid.toInt());
}

void PropViewBuilding::updateHeight(const QString &height) {
    static_cast<Building*>(m_mapEntity)->setHeight(height.toDouble());
}

void PropViewBuilding::updateAddress(const QString &address){
    static_cast<Building*>(m_mapEntity)->setAddress(address);
}

void PropViewBuilding::updatePostCode(const QString &postCode){
    static_cast<Building*>(m_mapEntity)->setPostCode(postCode);
}

void PropViewBuilding::updateLatitudeEdit(const QString & lat){
    static_cast<Building*>(m_mapEntity)->setLatitude(lat.toDouble());
}

void PropViewBuilding::updateLongitudeEdit(const QString &lng){
    static_cast<Building*>(m_mapEntity)->setLongitude(lng.toDouble());
}

void PropViewBuilding::updateTimeEdit(const QString &time){
    static_cast<Building*>(m_mapEntity)->setTime(time);
}

void PropViewBuilding::updateTelEdit(const QString &tel){
    static_cast<Building*>(m_mapEntity)->setTel(tel);
}
