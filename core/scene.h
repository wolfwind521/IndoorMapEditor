/****************************************************************************
** Scene
** @brief: manage the scene. provide interface to modify the mapEntities.
** @author: gaimeng
** @date: Dec, 2014
**
****************************************************************************/
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class MapEntity;
class Building;
class Floor;
class FuncArea;
class PubPoint;
class ImageLayer;
class PolygonEntity;
QT_FORWARD_DECLARE_CLASS(QGraphicsSceneMouseEvent)

enum DATA_TYPE{
    BUILDING,
    FLOOR,
    FUNCAREA,
    PUBPOINT,
    NONE
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:

    explicit Scene(QObject *parent = 0);
    void reset();
    void setSelectable(bool b);
    MapEntity* root() const;
    void createRoot();
    Building* building() const;
    void setBuilding(Building* building);

    //void addEntityByContext(PolygonEntity* polygon);
    PolygonEntity* createPolygonByContext();
    void deletePolygonByContext(PolygonEntity* entity);

    void addFloor(Floor* floor);
    void addFuncArea(FuncArea* funcArea);
    void addPubPoint(PubPoint* pubPoint);
    void addImageLayer(ImageLayer* imageLayer);

    void deleteSelected();
    void deleteMapEntity(MapEntity *entity);
    void removeMapEntity(MapEntity *entity);

    bool showFloor(int floorId);
    bool showDefaultFloor();
    Floor* currentFloor() const;
    void setCurrentFloor(Floor* floor);
signals:
    void buildingChanged();
public slots:
    void convertSelectedToBuilding();
    void convertSelectedToFloor();
    void convertSelectedToFuncArea();
protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

    //context menu
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
private:
    bool m_selectable;
    MapEntity *m_root;
    Building *m_building;
    Floor *m_curFloor;
};

#endif // SCENE_H
