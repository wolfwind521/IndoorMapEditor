#include "propviewroom.h"


#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>

#pragma comment(lib,"Qt5Widgets.lib")
#pragma execution_character_set("utf-8")

PropViewRoom::PropViewRoom(Feature *mapFeature, QWidget *parent) :
    PropertyView(mapFeature, parent)
{
    m_room = static_cast<Room*>(mapFeature);

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

PropViewRoom::~PropViewRoom(){

}

bool PropViewRoom::match(const Feature *mapFeature) const {
    return mapFeature->isClassOf("Room");
}

void PropViewRoom::updateWidgets(){
    PropertyView::updateWidgets();
    m_room = static_cast<Room*>(m_mapFeature);

    m_shopNoEdit->setText(m_room->shopNo());
    m_areaEdit->setText(QString::number(m_room->area()));
    m_dianpingIdEdit->setText(QString::number(m_room->dianpingId()));
    m_mateIdEdit->setText(QString::number(m_room->mateId()));
    m_sortComboBox->setCurrentText(getSortTypeName(m_room->sortType()));
    m_vacancyCheckBox->setChecked(m_room->areaStatus() == Room::Vacancy);
}

void PropViewRoom::updateShopNo(const QString &shopNo){
    m_room->setShopNo(shopNo);
}

void PropViewRoom::updateArea(const QString &area) {
    m_room->setArea(area.toDouble());
}

void PropViewRoom::updateDianpingId(const QString &dpId) {
    m_room->setDianpingId(dpId.toInt());
}

void PropViewRoom::updateMateId(const QString &mateId){
    m_room->setMateId(mateId.toInt());
}

void PropViewRoom::updateSortType(const QString &sortType){
    Room::SORT_TYPE type;
    if(!sortType.compare("边铺")){
        type = Room::SIDE_AREA;
    }else if(!sortType.compare("中岛")){
        type = Room::MIDDLE_AREA;
    }else {
        type = Room::UNSORTED;
    }
    m_room->setSortType(type);
}

void PropViewRoom::updateAreaStatus(const int state){
    if(state == Qt::Checked)
        m_room->setAreaStatus(Room::Vacancy);
    else if(state == Qt::Unchecked)
        m_room->setAreaStatus(Room::Working);
}

void PropViewRoom::queryFinished(){
//    m_webDlg->close();
//    delete m_webDlg;
//    m_webDlg = NULL;
}

void PropViewRoom::onQuery(){

}

void PropViewRoom::addJsObject(){

}


QString PropViewRoom::getSortTypeName(Room::SORT_TYPE sortType){
    if(sortType == Room::UNSORTED)
        return "无分类";
    if(sortType == Room::MIDDLE_AREA)
        return "中岛";
    if(sortType == Room::SIDE_AREA)
        return "边铺";
}
