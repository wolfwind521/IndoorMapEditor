/***************************************************
 ** PropViewFuncArea
 ** @brief: the property view for funcAreas
 ** @author: gaimeng
 ** @date: Jan, 2015
 **
 ****************************************************/
#ifndef PROPVIEWFUNCAREA_H
#define PROPVIEWFUNCAREA_H

#include "propertyview.h"
#include "../core/funcarea.h"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QWebView)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class PropViewFuncArea : public PropertyView
{
    Q_OBJECT
public:
    explicit PropViewFuncArea(MapEntity *mapEntity, QWidget *parent = 0);
    ~PropViewFuncArea();
    virtual bool match(const MapEntity *mapEntity) const;
    virtual void updateWidgets();
public slots:
   void updateShopNo(const QString &shopNo);
   void updateArea(const QString &area);
   void updateDianpingId(const QString &dpId);
   void updateMateId(const QString &mateId);
   void onQuery();
   void queryFinished();
   void addJsObject();
   void onCheckDianpingId();
   void updateSortType(const QString &sortType);
   void updateAreaStatus(const int state);

private:
   QString getSortTypeName(FuncArea::SORT_TYPE sortType);

    QPushButton *m_queryButton;
    QLineEdit *m_shopNoEdit;
    QLineEdit *m_areaEdit;
    QLineEdit *m_dianpingIdEdit;
    QPushButton *m_checkDianpingBtn;
    QLineEdit *m_mateIdEdit;
    QWebView *m_webDlg;
    FuncArea *m_funcArea;
    QComboBox * m_typeComboBox; //业态类别
    QComboBox * m_sortComboBox; //铺位类别
    QCheckBox * m_vacancyCheckBox;
};

#endif // PROPVIEWFUNCAREA_H
