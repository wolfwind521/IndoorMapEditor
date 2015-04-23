#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QGraphicsView>
#include <QModelIndex>

QT_FORWARD_DECLARE_CLASS(QUndoStack)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGraphicsObject)
QT_FORWARD_DECLARE_CLASS(QPrinter)

class Floor;
class Building;
class MapEntity;
class Scene;

class DocumentView : public QGraphicsView
{

    Q_OBJECT
public:
    enum ViewFlag{
        StyleShowShopName =  0x00000001,
        StyleShowCenter =    0x00000002,
        StyleShowPointText = 0x00000004,
        StyleShowDirection =      0x00000008,
        //TODO more styles
        StyleDefault = 0 | StyleShowShopName | StyleShowCenter | StyleShowPointText
    };
    Q_DECLARE_FLAGS(ViewStyle, ViewFlag)

    DocumentView();
    ~DocumentView();
    Scene * scene() const;
    bool isModified();
    void setModified(bool b);
    void clear();
    void setSelectable(bool b);
    bool selectable() const;
    Building* building();
    static ViewStyle viewStyle();
    void zoom(int step);
    void addScale(double s);
    double getScale() const;

signals:
    void selectionChanged(MapEntity * mapEntity);

public slots:
    void printScene(QPrinter *printer);
    void printCurrentView(QPrinter *printer);
    void updateSelection(const QModelIndex & index);
    void updateSelection();
    void showShopText(bool show);
    void showPointText(bool show);
    void showDirection(bool show);
    //zooming
    void zoomIn(int step = 2);
    void zoomOut(int step = 2);
    void fitView();
    void onRotate();
    void onFlip();

protected:
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QUndoStack *m_undoStack;
    Scene *m_scene;
    bool m_isModified;
    bool m_selectable;
    static ViewStyle m_style;
    bool m_ctrlKeyPressed;
    int m_scale;
    double m_scaleSum;
    int m_scaleNum;
};

#endif // DOCUMENTVIEW_H
