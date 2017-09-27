#ifndef PROPERTYVIEW_H
#define PROPERTYVIEW_H

#include <QWidget>

class Feature;
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QFormLayout)
QT_FORWARD_DECLARE_CLASS(QComboBox)

class PropertyView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyView(Feature *mapFeature, QWidget *parent = 0);
    ~PropertyView();

    virtual bool match(const Feature * mapFeature) const;
    virtual void setMapFeature( Feature * mapFeature);
    virtual void updateWidgets();
public slots:  
    void updateName(const QString &name);
    void updateEnName(const QString &enName);
    void updateId(const QString &id);
    void updateBrief(const QString &brief);
    void updateType(const QString &type);

protected:
    void createWidgets();
    void createLayout();
    void createConnections();

    Feature * m_mapFeature;
    QFormLayout * m_layout;

    QLineEdit * m_nameLineEdit;
    QLineEdit * m_enNameLineEdit;
    QComboBox * m_typeComboBox;
    QLineEdit * m_idEdit;
    QLineEdit * m_briefEdit;

};

#endif // PROPERTYVIEW_H
