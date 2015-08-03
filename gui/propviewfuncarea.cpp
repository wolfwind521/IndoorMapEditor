#include "propviewfuncarea.h"
#include "../core/funcarea.h"

#include <QLineEdit>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>

#pragma comment(lib,"Qt5Widgets.lib")
#pragma comment(lib,"Qt5WebKitWidgets.lib")
#pragma execution_character_set("utf-8")

PropViewFuncArea::PropViewFuncArea(MapEntity *mapEntity, QWidget *parent) :
    PropertyView(mapEntity, parent), m_webDlg(NULL)
{
    m_funcArea = static_cast<FuncArea*>(mapEntity);

    m_shopNoEdit = new QLineEdit;
    m_areaEdit = new QLineEdit;
    m_dianpingIdEdit = new QLineEdit;
    m_queryButton = new QPushButton(tr("品牌关联"));
    m_checkDianpingBtn = new QPushButton(tr("检查"));
    m_mateIdEdit = new QLineEdit;

    m_layout->addRow(tr("<b><font color=red>铺位号</font></b>"), m_shopNoEdit);
    m_layout->addRow(tr("面积（平方米）"), m_areaEdit);
    QHBoxLayout *dianpingLayout = new QHBoxLayout();
    dianpingLayout->addWidget(m_dianpingIdEdit);
    dianpingLayout->addWidget(m_checkDianpingBtn);
    m_layout->addRow(tr("<b><font color=red>点评 ID</font></b>"), dianpingLayout);
    m_layout->insertRow(0,m_queryButton);
    m_layout->addRow(tr("同铺id"), m_mateIdEdit);

    updateWidgets();

    connect(m_shopNoEdit, SIGNAL(textEdited(QString)), this, SLOT(updateShopNo(QString)));
    connect(m_areaEdit, SIGNAL(textEdited(QString)), this, SLOT(updateArea(QString)));
    connect(m_dianpingIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDianpingId(QString)));
    connect(m_queryButton, SIGNAL(clicked()), this, SLOT(onQuery()));
    connect(m_mateIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateMateId(QString)) );
    connect(m_checkDianpingBtn, SIGNAL(clicked()), this, SLOT(onCheckDianpingId()));
}

PropViewFuncArea::~PropViewFuncArea(){
    if(m_webDlg != NULL){
        delete m_webDlg;
        m_webDlg = NULL;
    }
}

bool PropViewFuncArea::match(const MapEntity *mapEntity) const {
    return mapEntity->isClassOf("FuncArea");
}

void PropViewFuncArea::updateWidgets(){
    PropertyView::updateWidgets();

    m_shopNoEdit->setText(m_funcArea->shopNo());
    m_areaEdit->setText(QString::number(m_funcArea->area()));
    m_dianpingIdEdit->setText(QString::number(m_funcArea->dianpingId()));
    m_mateIdEdit->setText(QString::number(m_funcArea->mateId()));
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

void PropViewFuncArea::queryFinished(){
//    m_webDlg->close();
//    delete m_webDlg;
//    m_webDlg = NULL;
}

void PropViewFuncArea::onQuery(){
    if(m_webDlg != NULL){
        delete m_webDlg;
    }
    FuncArea* funcArea = static_cast<FuncArea*>(m_mapEntity);
    m_webDlg = new QWebView();
    QObject::connect(m_webDlg->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
                     this, SLOT(addJsObject()));
    QUrl url("http://admin.fangcheng.cn/#/search/"+funcArea->objectName()+"/mapId/"+QString::number(funcArea->type().toInt())+"/");

    m_webDlg->setUrl(url);
    m_webDlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_webDlg->show();
}

void PropViewFuncArea::addJsObject(){
    m_webDlg->page()->mainFrame()->addToJavaScriptWindowObject(QString("indoorMap"), this);
}

void PropViewFuncArea::onCheckDianpingId(){
    if(m_webDlg != NULL){
        delete m_webDlg;
    }
    m_webDlg = new QWebView();
    QUrl url("http://www.dianping.com/shop/" + QString::number(static_cast<FuncArea*>(m_mapEntity)->dianpingId()));
    //QUrl url("http://wolfwind521.github.io/2dmap");
    m_webDlg->setUrl(url);
    m_webDlg->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_webDlg->show();
}
