#include "propviewfuncarea.h"
#include "../core/funcarea.h"
#include <QLineEdit>
#include <QFormLayout>

#pragma execution_character_set("utf-8")

PropViewFuncArea::PropViewFuncArea(QWidget *parent) :
    PropertyView(parent)
{
    m_shopNoEdit = new QLineEdit;
    m_areaEdit = new QLineEdit;
    m_dianpingIdEdit = new QLineEdit;

    m_layout->addRow(tr("铺位号"), m_shopNoEdit);
    m_layout->addRow(tr("面积"), m_areaEdit);
    m_layout->addRow(tr("点评 ID"), m_dianpingIdEdit);

    connect(m_shopNoEdit, SIGNAL(textEdited(QString)), this, SLOT(updateShopNo(QString)));
    connect(m_areaEdit, SIGNAL(textEdited(QString)), this, SLOT(updateArea(QString)));
    connect(m_dianpingIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDianpingId(QString)));
}

bool PropViewFuncArea::match(const MapEntity *mapEntity) const {
    return mapEntity->isClassOf("FuncArea");
}

void PropViewFuncArea::setMapEntity(MapEntity *mapEntity){
    if(mapEntity != m_mapEntity) {
        PropertyView::setMapEntity(mapEntity);
        FuncArea* funcArea = static_cast<FuncArea*>(mapEntity);
        m_shopNoEdit->setText(funcArea->shopNo());
        m_areaEdit->setText(QString::number(funcArea->area()));
        m_dianpingIdEdit->setText(QString::number(funcArea->dianpingId()));
    }
}

void PropViewFuncArea::updateShopNo(const QString &shopNo){
    static_cast<FuncArea*>(m_mapEntity)->setShopNo(shopNo);
}

void PropViewFuncArea::updateArea(const QString &area) {
    static_cast<FuncArea*>(m_mapEntity)->setArea(area.toDouble());
}

void PropViewFuncArea::updateDianpingId(const QString &dpId) {
    static_cast<FuncArea*>(m_mapEntity)->setDianpingId(dpId.toInt());
}
