#ifndef PROPVIEWFLOOR_H
#define PROPVIEWFLOOR_H

#include "propertyview.h"

class Floor;
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QLabel)


class PropViewFloor : public PropertyView
{
    Q_OBJECT
public:
    PropViewFloor(Feature *mapFeature, QWidget *parent = 0);
    virtual bool match(const Feature *mapFeature) const;
    virtual void updateWidgets();

public slots:
    void updateHeight(const QString &height);

private:
    QLineEdit * m_heightEdit;
    QLabel * m_funcAreaNumLabel;
    QLabel * m_pubPointNumLabel;

};

#endif // PROPVIEWFLOOR_H
