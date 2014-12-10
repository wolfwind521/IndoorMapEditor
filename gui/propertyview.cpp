#include "propertyview.h"
#include "../core/mapentity.h"
#include <QFormLayout>
#include <QLineEdit>

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
}

void PropertyView::createLayout(){
    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Name"), m_nameLineEdit);
    layout->addRow(tr("EnglishName"), m_enNameLineEdit);
    setLayout(layout);
}

void PropertyView::createConnections(){
    connect(m_nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateName(QString)));
    connect(m_enNameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(updateEnName(QString)));
}

void PropertyView::setMapEntity(MapEntity * mapEntity){
    if(m_mapEntity != mapEntity){
        m_mapEntity = mapEntity;
        if(m_mapEntity != NULL){
            m_nameLineEdit->setText(m_mapEntity->objectName());
            m_enNameLineEdit->setText(m_mapEntity->enName());
        }else{
            m_nameLineEdit->setText(QString());
            m_enNameLineEdit->setText(QString());
        }
    }
}

void PropertyView::updateName(const QString &name){
    m_mapEntity->setObjectName(name);
}

void PropertyView::updateEnName(const QString &enName){
    m_mapEntity->setEnName(enName);
}




