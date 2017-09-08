#include "propviewfuncarea.h"


#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>

#pragma comment(lib,"Qt5Widgets.lib")
#pragma execution_character_set("utf-8")

PropViewFuncArea::PropViewFuncArea(MapEntity *mapEntity, QWidget *parent) :
    PropertyView(mapEntity, parent)
{
    m_funcArea = static_cast<FuncArea*>(mapEntity);

    m_shopNoEdit = new QLineEdit;
    m_areaEdit = new QLineEdit;
    m_dianpingIdEdit = new QLineEdit;
    m_queryButton = new QPushButton(tr("品牌关联"));
    m_checkDianpingBtn = new QPushButton(tr("检查"));
    m_mateIdEdit = new QLineEdit;
    m_sortComboBox = new QComboBox;
    QStringList strlist ;
    strlist<<"边铺"<<"中岛"<<"无分类";
    m_sortComboBox->addItems(strlist);
    m_vacancyCheckBox = new QCheckBox;
    m_vacancyCheckBox->setText(tr("空铺"));


    m_layout->addRow(tr("<b><font color=red>铺位号</font></b>"), m_shopNoEdit);
    m_layout->addRow(tr("面积（平方米）"), m_areaEdit);
    QHBoxLayout *dianpingLayout = new QHBoxLayout();
    dianpingLayout->addWidget(m_dianpingIdEdit);
    dianpingLayout->addWidget(m_checkDianpingBtn);
    m_layout->addRow(tr("<b><font color=red>点评 ID</font></b>"), dianpingLayout);
    m_layout->insertRow(0,m_queryButton);
    m_layout->addRow(tr("同铺id"), m_mateIdEdit);
    m_layout->addRow(tr("铺位类别"), m_sortComboBox);
    m_layout->addRow(tr("铺位状态"), m_vacancyCheckBox);

    updateWidgets();

    connect(m_shopNoEdit, SIGNAL(textEdited(QString)), this, SLOT(updateShopNo(QString)));
    connect(m_areaEdit, SIGNAL(textEdited(QString)), this, SLOT(updateArea(QString)));
    connect(m_dianpingIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDianpingId(QString)));
    connect(m_queryButton, SIGNAL(clicked()), this, SLOT(onQuery()));
    connect(m_mateIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateMateId(QString)) );
    connect(m_sortComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSortType(QString)));
    connect(m_vacancyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateAreaStatus(int)));
}

PropViewFuncArea::~PropViewFuncArea(){

}

bool PropViewFuncArea::match(const MapEntity *mapEntity) const {
    return mapEntity->isClassOf("FuncArea");
}

void PropViewFuncArea::updateWidgets(){
    PropertyView::updateWidgets();
    m_funcArea = static_cast<FuncArea*>(m_mapEntity);

    m_shopNoEdit->setText(m_funcArea->shopNo());
    m_areaEdit->setText(QString::number(m_funcArea->area()));
    m_dianpingIdEdit->setText(QString::number(m_funcArea->dianpingId()));
    m_mateIdEdit->setText(QString::number(m_funcArea->mateId()));
    m_sortComboBox->setCurrentText(getSortTypeName(m_funcArea->sortType()));
    m_vacancyCheckBox->setChecked(m_funcArea->areaStatus() == FuncArea::Vacancy);
}

void PropViewFuncArea::updateShopNo(const QString &shopNo){
    m_funcArea->setShopNo(shopNo);
}

void PropViewFuncArea::updateArea(const QString &area) {
    m_funcArea->setArea(area.toDouble());
}

void PropViewFuncArea::updateDianpingId(const QString &dpId) {
    m_funcArea->setDianpingId(dpId.toInt());
}

void PropViewFuncArea::updateMateId(const QString &mateId){
    m_funcArea->setMateId(mateId.toInt());
}

void PropViewFuncArea::updateSortType(const QString &sortType){
    FuncArea::SORT_TYPE type;
    if(!sortType.compare("边铺")){
        type = FuncArea::SIDE_AREA;
    }else if(!sortType.compare("中岛")){
        type = FuncArea::MIDDLE_AREA;
    }else {
        type = FuncArea::UNSORTED;
    }
    m_funcArea->setSortType(type);
}

void PropViewFuncArea::updateAreaStatus(const int state){
    if(state == Qt::Checked)
        m_funcArea->setAreaStatus(FuncArea::Vacancy);
    else if(state == Qt::Unchecked)
        m_funcArea->setAreaStatus(FuncArea::Working);
}

void PropViewFuncArea::queryFinished(){
//    m_webDlg->close();
//    delete m_webDlg;
//    m_webDlg = NULL;
}

void PropViewFuncArea::onQuery(){

}

void PropViewFuncArea::addJsObject(){

}


QString PropViewFuncArea::getSortTypeName(FuncArea::SORT_TYPE sortType){
    if(sortType == FuncArea::UNSORTED)
        return "无分类";
    if(sortType == FuncArea::MIDDLE_AREA)
        return "中岛";
    if(sortType == FuncArea::SIDE_AREA)
        return "边铺";
}
