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

class Feature;
class Building;
class Floor;
class Room;
class PubPoint;
class ImageLayer;
class PolygonFeature;
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
    Feature* root() const;
    void createRoot();
    Building* building() const;
    void setBuilding(Building* building);

    //void addFeatureByContext(PolygonFeature* polygon);
    PolygonFeature* createPolygonByContext();
    void deletePolygonByContext(PolygonFeature* feature);

    Floor* addFloor(Floor* floor = 0);
    Room* addRoom(Room* room);
    void addPubPoint(PubPoint* pubPoint);
    void addImageLayer(ImageLayer* imageLayer);

    void deleteSelectedItems();
    void deleteSelectedLayers();
    void deleteMapFeature(Feature *feature);
    void removeMapFeature(Feature *feature);

    bool showFloor(int floorId);
    bool showDefaultFloor();
    Floor* currentFloor() const;
    void setCurrentFloor(Floor* floor);

    QList<Feature *> findMapFeature(const QString & name);
    void selectMapFeature(Feature* feature); //select the room and change the floor

    QList<QList<Feature*> > findAllRepeat();

    void transformMap(const QTransform &matrix);
    void addScale(double s);
    void clearSelectedLayers();
    void setSelectedLayer(Feature *feature);
    Feature* currentLayer();
signals:
    void buildingChanged();
    void fontChanged(const QFont &font);
public slots:
    void convertSelectedToBuilding();
    void convertSelectedToFloor();
    void convertSelectedToRoom();
protected:
    //virtual void event(QEvent *event);
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    bool event(QEvent *event);
    //context menu
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
private:
    bool m_selectable;
    Feature *m_root;
    Building *m_building;
    Floor *m_curFloor;
    double m_scaleSum;
    int m_scaleNum;
    double m_curScale;
    QList<Feature*> m_selectedLayers;
};

#endif // SCENE_H
