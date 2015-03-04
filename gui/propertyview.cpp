#include "propertyview.h"
#include "../core/mapentity.h"
#include <QFormLayout>
#include <QLineEdit>

#pragma execution_character_set("utf-8")

PropertyView::PropertyView(QWidget *parent) :
    QWidget(parent), m_mapEntity(NULL)
{
    createWidgets();
    createLayout();
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
    m_typeEdit = new QLineEdit;
}

void PropertyView::createLayout(){
    m_layout = new QFormLayout;
    m_layout->addRow(tr("<b><font color=red>名称</font></b>"), m_nameLineEdit);
    m_layout->addRow(tr("<b><font color=red>英文名</font></b>"), m_enNameLineEdit);
    m_layout->addRow(tr("id"), m_idEdit);
    m_layout->addRow(tr("类型"), m_typeEdit);
    m_layout->addRow(tr("<b><font color=red>备注</font></b>"), m_briefEdit);
    setLayout(m_layout);
}

void PropertyView::createConnections(){
    connect(m_nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateName(QString)));
    connect(m_enNameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateEnName(QString)));
    connect(m_idEdit, SIGNAL(textEdited(QString)), this, SLOT(updateId(QString)));
    connect(m_briefEdit, SIGNAL(textEdited(QString)), this, SLOT(updateBrief(QString)));
    connect(m_typeEdit, SIGNAL(textEdited(QString)), this, SLOT(updateType(QString)));
}

bool PropertyView::match(const MapEntity *mapEntity) const {
    return false;
}
void PropertyView::setMapEntity(MapEntity * mapEntity){
    if(m_mapEntity != mapEntity){
        m_mapEntity = mapEntity;
        if(m_mapEntity != NULL){
            m_nameLineEdit->setText(m_mapEntity->objectName());
            m_enNameLineEdit->setText(m_mapEntity->enName());
            m_idEdit->setText(QString::number(m_mapEntity->id()));
            m_briefEdit->setText(m_mapEntity->brief());
            m_typeEdit->setText(m_mapEntity->type());
        }else{
            m_nameLineEdit->setText(QString());
            m_enNameLineEdit->setText(QString());
            m_idEdit->setText(QString());
            m_briefEdit->setText(QString());
            m_typeEdit->setText(QString());
        }
    }
}

void PropertyView::updateName(const QString &name){
    m_mapEntity->setObjectName(name);
    m_mapEntity->update(m_mapEntity->boundingRect()); //redraw
}

void PropertyView::updateEnName(const QString &enName){
    m_mapEntity->setEnName(enName);
}

void PropertyView::updateId(const QString &id) {
    m_mapEntity->setId(id.toInt());
}

void PropertyView::updateBrief(const QString &brief) {
    m_mapEntity->setBrief(brief);
    m_briefEdit->setText(brief);
}

void PropertyView::updateType(const QString &type) {
    m_mapEntity->setType(type);
}

