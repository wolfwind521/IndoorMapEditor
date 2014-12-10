#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QGraphicsView>
#include <QModelIndex>

QT_FORWARD_DECLARE_CLASS(QUndoStack)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsObject)
QT_FORWARD_DECLARE_CLASS(QPainter)
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
    QGraphicsScene * scene() const;
    void setBuilding(Building* building);
    bool isModified();
    void setModified(bool b);
    void clear();
    void printScene(QPainter *painter);

signals:
    void buildingChanged(const Building & building);
    void selectionChanged(MapEntity * mapEntity);

public slots:
    void updateSelection(const QModelIndex & index);
    void updateSelection();

private:
    QUndoStack *m_undoStack;
    QGraphicsScene *m_scene;
    MapEntity *m_root;
    Building *m_building;
    bool m_isModified;
};

#endif // DOCUMENTVIEW_H
