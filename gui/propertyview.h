#ifndef PROPERTYVIEW_H
#define PROPERTYVIEW_H

#include <QWidget>

class MapEntity;
QT_FORWARD_DECLARE_CLASS(QLineEdit)

class PropertyView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyView(QWidget *parent = 0);
    ~PropertyView();

public slots:
    void setMapEntity( MapEntity * mapEntity);
    void updateName(const QString &name);
    void updateEnName(const QString &enName);
private:
    void createWidgets();
    void createLayout();
    void createConnections();

    MapEntity* m_mapEntity;
    QLineEdit* m_nameLineEdit;
    QLineEdit* m_enNameLineEdit;
};

#endif // PROPERTYVIEW_H
