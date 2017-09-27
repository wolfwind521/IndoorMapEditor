#include "propviewfloor.h"
#include "../core/floor.h"
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#pragma execution_character_set("utf-8")

PropViewFloor::PropViewFloor(Feature *mapFeature, QWidget *parent) :
    PropertyView(mapFeature, parent)
{
    m_heightEdit = new QLineEdit;
    m_funcAreaNumLabel = new QLabel;
    m_pubPointNumLabel = new QLabel;

    updateWidgets();

    m_layout->addRow(tr("高度"), m_heightEdit);
    m_layout->addRow(tr("店铺数 "), m_funcAreaNumLabel);
    m_layout->addRow(tr("设施数"), m_pubPointNumLabel);

    connect(m_heightEdit, SIGNAL(textEdited(QString)), this, SLOT(updateHeight(QString)));
}

bool PropViewFloor::match(const Feature *mapFeature) const {
    return mapFeature->isClassOf("Floor");
}

void PropViewFloor::updateWidgets(){
    PropertyView::updateWidgets();
    Floor *floor = static_cast<Floor*>(m_mapFeature);
    m_heightEdit->setText(QString::number(floor->height()));
}

void PropViewFloor::updateHeight(const QString &height) {
    static_cast<Floor*>(m_mapFeature)->setHeight(height.toDouble());
}
