#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QGraphicsView>
#include <QModelIndex>

QT_FORWARD_DECLARE_CLASS(QUndoStack)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsObject)
class Building;
class MapEntity;

class DocumentView : public QGraphicsView
{

    Q_OBJECT
public:
    DocumentView();
    ~DocumentView();
    MapEntity* root() const;
    Building* building() const;
    void setBuilding(Building* building);
    bool isModified();
    void setModified(bool b);
    void clear();

signals:
    void buildingChanged(const Building & building);

public slots:
    void selectEntity(const QModelIndex & index);
private:
    QUndoStack *m_undoStack;
    QGraphicsScene *m_scene;
    MapEntity *m_root;
    Building *m_building;
    bool m_isModified;
};

#endif // DOCUMENTVIEW_H
