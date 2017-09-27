#include "propertyview.h"
#include "../core/feature.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>

#pragma execution_character_set("utf-8")

PropertyView::PropertyView(Feature *mapFeature, QWidget *parent) :
    QWidget(parent), m_mapFeature(mapFeature)
{
    createWidgets();
    createLayout();
    updateWidgets();
    createConnections();
}

PropertyView::~PropertyView()
{
}

void PropertyView::createWidgets(){
    m_nameLineEdit = new QLineEdit;
    m_enNameLineEdit = new QLineEdit;
    m_idEdit = new QLineEdit;
    m_briefEdit = new QLineEdit;
    m_typeComboBox = new QComboBox;
    QStringList strlist = m_mapFeature->typeStringList();
    if(strlist.empty()){
        m_typeComboBox->hide();
    }else{
        m_typeComboBox->addItems(strlist);
    }
}

void PropertyView::createLayout(){
    m_layout = new QFormLayout;
    m_layout->addRow(tr("<b><font color=red>名称</font></b>"), m_nameLineEdit);
    m_layout->addRow(tr("<b><font color=red>英文名</font></b>"), m_enNameLineEdit);
    m_layout->addRow(tr("id"), m_idEdit);
    if(m_typeComboBox->isHidden())
        m_layout->addRow(tr("类型"), m_typeComboBox);
    m_layout->addRow(tr("<b><font color=red>备注</font></b>"), m_briefEdit);
    setLayout(m_layout);
}

void PropertyView::createConnections(){
    connect(m_nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateName(QString)));
    connect(m_enNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateEnName(QString)));
    connect(m_idEdit, SIGNAL(textEdited(QString)), this, SLOT(updateId(QString)));
    connect(m_briefEdit, SIGNAL(textEdited(QString)), this, SLOT(updateBrief(QString)));
    connect(m_typeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateType(QString)));
}

bool PropertyView::match(const Feature *mapFeature) const {
    if(mapFeature->isClassOf("PubPoint"))
        return true;
    else
        return false;
}

void PropertyView::setMapFeature(Feature * mapFeature){
    if(m_mapFeature != mapFeature){
        m_mapFeature = mapFeature;
        updateWidgets();
    }
}

void PropertyView::updateWidgets(){
    if(m_mapFeature != NULL){
        m_nameLineEdit->setText(m_mapFeature->objectName());
        m_enNameLineEdit->setText(m_mapFeature->enName());
        m_idEdit->setText(QString::number(m_mapFeature->id()));
        m_briefEdit->setText(m_mapFeature->brief());
        m_typeComboBox->setCurrentText(m_mapFeature->getTypeName());
    }else{
        m_nameLineEdit->setText(QString());
        m_enNameLineEdit->setText(QString());
        m_idEdit->setText(QString());
        m_briefEdit->setText(QString());
        m_typeComboBox->clear();
    }
}

void PropertyView::updateName(const QString &name){
    m_mapFeature->setObjectName(name);
    m_mapFeature->update(m_mapFeature->boundingRect()); //redraw
}

void PropertyView::updateEnName(const QString &enName){
    m_mapFeature->setEnName(enName);
}

void PropertyView::updateId(const QString &id) {
    m_mapFeature->setId(id.toInt());
}

void PropertyView::updateBrief(const QString &brief) {
    m_mapFeature->setBrief(brief);
    m_briefEdit->setText(brief);
}

void PropertyView::updateType(const QString &typeName){
    m_mapFeature->updateByTypeName(typeName);
}
