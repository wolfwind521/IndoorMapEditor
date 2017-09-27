/***************************************************
 ** PropViewFuncArea
 ** @brief: the property view for funcAreas
 ** @author: gaimeng
 ** @date: Jan, 2015
 **
 ****************************************************/
#ifndef PROPVIEWROOM_H
#define PROPVIEWROOM_H

#include "propertyview.h"
#include "../core/room.h"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class PropViewRoom : public PropertyView
{
    Q_OBJECT
public:
    explicit PropViewRoom(Feature *mapFeature, QWidget *parent = 0);
    ~PropViewRoom();
    virtual bool match(const Feature *mapFeature) const;
    virtual void updateWidgets();
public slots:
   void updateShopNo(const QString &shopNo);
   void updateArea(const QString &area);
   void updateDianpingId(const QString &dpId);
   void updateMateId(const QString &mateId);
   void onQuery();
   void queryFinished();
   void addJsObject();
   void updateSortType(const QString &sortType);
   void updateAreaStatus(const int state);

private:
   QString getSortTypeName(Room::SORT_TYPE sortType);

    QPushButton *m_queryButton;
    QLineEdit *m_shopNoEdit;
    QLineEdit *m_areaEdit;
    QLineEdit *m_dianpingIdEdit;
    QPushButton *m_checkDianpingBtn;
    QLineEdit *m_mateIdEdit;
    Room *m_room;
    QComboBox * m_typeComboBox; //业态类别
    QComboBox * m_sortComboBox; //铺位类别
    QCheckBox * m_vacancyCheckBox;
};

#endif // PROPVIEWROOM_H
