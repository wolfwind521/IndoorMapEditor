#ifndef PROPVIEWBUILDING_H
#define PROPVIEWBUILDING_H

#include "propertyview.h"

class Feature;
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QFormLayout)

class PropViewBuilding : public PropertyView
{
    Q_OBJECT
public:
    explicit PropViewBuilding(Feature *mapFeature, QWidget *parent = 0);
    ~PropViewBuilding();

    virtual bool match(const Feature * mapFeature) const;
    virtual void updateWidgets();
public slots:
    void updateHeight(const QString &height);
    void updateAddress(const QString &address);
    void updatePostCode(const QString &postCode);
    void updateLatitudeEdit(const QString &lat);
    void updateLongitudeEdit(const QString &lng);
    void updateTimeEdit(const QString & time);
    void updateTelEdit(const QString & tel);

private:
    QLabel *m_underFloorsLabel;
    QLabel *m_groundFloorsLabel;

    QLineEdit *m_heightEdit;
    QLineEdit *m_addressEdit;
    QLineEdit *m_postCodeEdit;
    QLineEdit *m_latitudeEdit;
    QLineEdit *m_longitudeEdit;
    QLineEdit *m_timeEdit;
    QLineEdit *m_telEdit;
};

#endif // PROPVIEWBUILDING_H
