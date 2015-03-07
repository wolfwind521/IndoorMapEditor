#include "propviewfuncarea.h"
#include "../core/funcarea.h"

#include <QLineEdit>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QPushButton>
#include <QFormLayout>

#pragma comment(lib,"Qt5Widgets.lib")
#pragma comment(lib,"Qt5WebKitWidgets.lib")
#pragma execution_character_set("utf-8")

PropViewFuncArea::PropViewFuncArea(QWidget *parent) :
    PropertyView(parent), m_webDlg(NULL)
{
    m_shopNoEdit = new QLineEdit;
    m_areaEdit = new QLineEdit;
    m_dianpingIdEdit = new QLineEdit;
    m_queryButton = new QPushButton(tr("品牌关联"));

    m_layout->addRow(tr("<b><font color=red>铺位号</font></b>"), m_shopNoEdit);
    m_layout->addRow(tr("面积（平方米）"), m_areaEdit);
    m_layout->addRow(tr("<b><font color=red>点评 ID</font></b>"), m_dianpingIdEdit);
    m_layout->insertRow(0,m_queryButton);

    connect(m_shopNoEdit, SIGNAL(textEdited(QString)), this, SLOT(updateShopNo(QString)));
    connect(m_areaEdit, SIGNAL(textEdited(QString)), this, SLOT(updateArea(QString)));
    connect(m_dianpingIdEdit, SIGNAL(textEdited(QString)), this, SLOT(updateDianpingId(QString)));
    connect(m_queryButton, SIGNAL(clicked()), this, SLOT(onQuery()));
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
