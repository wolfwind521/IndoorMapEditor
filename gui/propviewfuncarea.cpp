#include "propviewfuncarea.h"
#include "../core/funcarea.h"
#include <QLineEdit>
#include <QFormLayout>

PropViewFuncArea::PropViewFuncArea(QWidget *parent) :
    PropertyView(parent)
{
    m_shopNoEdit = new QLineEdit;
    m_areaEdit = new QLineEdit;

    m_layout->addRow(tr("Shop No."), m_shopNoEdit);
    m_layout->addRow(tr("Area"), m_areaEdit);

    connect(m_shopNoEdit, SIGNAL(textEdited(QString)), this, SLOT(updateShopNo(QString)));
    connect(m_areaEdit, SIGNAL(textEdited(QString)), this, SLOT(updateArea(QString)));
}

bool PropViewFuncArea::match(const MapEntity *mapEntity) const {
    QString className = mapEntity->metaObject()->className();
    if(className == "FuncArea") {
        return true;
    }else{
        return false;
    }
}

void PropViewFuncArea::setMapEntity(MapEntity *mapEntity){
    PropertyView::setMapEntity(mapEntity);
    FuncArea* funcArea = static_cast<FuncArea*>(mapEntity);
    m_shopNoEdit->setText(funcArea->shopNo());
    m_areaEdit->setText(QString::number(funcArea->area()));
}

void PropViewFuncArea::updateShopNo(const QString &shopNo){
    static_cast<FuncArea*>(m_mapEntity)->setShopNo(shopNo);
}

void PropViewFuncArea::updateArea(const QString &area) {
    static_cast<FuncArea*>(m_mapEntity)->setArea(area.toDouble());
}
