#include "documentview.h"
#include "../core/building.h"
#include "../core/mapentity.h"
#include "../core/floor.h"
#include "../core/funcarea.h"
#include "../core/pubpoint.h"
#include "../core/scene.h"
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QUndoStack>
#include <QKeyEvent>
#include <QtPrintSupport/QPrinter>
#include <QApplication>
#include <qmath.h>


DocumentView::ViewStyle DocumentView::m_style = StyleDefault;

DocumentView::DocumentView()
    :m_isModified(false), m_selectable(true), m_ctrlKeyPressed(false), m_scale(0)
{
    m_scene = new Scene(this);
    m_scene->reset();
    m_undoStack = new QUndoStack(this);

    this->setScene(m_scene);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHints(QPainter::Antialiasing);

    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

DocumentView::~DocumentView()
{
}

bool DocumentView::isModified()
{
    return m_isModified;
}

void DocumentView::setModified(bool b)
{
    m_isModified = b;
}

Building* DocumentView::building(){
    return m_scene->building();
}

void DocumentView::clear()
{
    m_scene->reset();
    m_undoStack->clear();
}

void DocumentView::printScene(QPrinter *printer){
    printer->setFromTo(1, m_scene->building()->floorNum());

    QPainter painter;
    painter.begin(printer);
    QFont font = QApplication::font("DocumentView");//save the old font
    QFont printFont = QFont(font, painter.device());
    QApplication::setFont(printFont, "DocumentView");
    QRectF rect = m_scene->building()->boundingRect();
    bool firstPage = true;
    for(int page = printer->fromPage(); page <= printer->toPage(); ++page){
        if(!firstPage)
            printer->newPage();
        QVector<Floor*> floors = m_scene->building()->getFloors();
        m_scene->showFloor(floors[page-1]->id());
        QRectF newRect = rect.united(m_scene->currentFloor()->boundingRect()); //unite the building rect with the floor rect
        m_scene->render(&painter, printer->pageRect(), newRect, Qt::KeepAspectRatio);
        firstPage = false;
    }
    painter.end();

    QApplication::setFont(font, "DocumentView");//restore the old font
}

//selection from graphics view
void DocumentView::updateSelection(){
    if(m_scene->selectedItems().size() > 0){
        MapEntity* selectedEntity = static_cast<MapEntity*>(m_scene->selectedItems().at(0));
        emit selectionChanged(selectedEntity);
    }else{
        emit selectionChanged(NULL);
    }
}

//selection from tree view
void DocumentView::updateSelection(const QModelIndex & index){
    MapEntity *mapEntity = static_cast<MapEntity*>(index.internalPointer());

    //a floor selected, change the visible floor
    QString className = mapEntity->metaObject()->className();
    if( className == "Floor"){
        QObject *floor;
        foreach (floor, m_scene->building()->children()) {
            static_cast<MapEntity*>(floor)->setVisible(false);
        }
        mapEntity->setVisible(true);
        m_scene->setCurrentFloor(static_cast<Floor*>(mapEntity));
        this->update();
    }
    //a funcArea or pubPoint selected, change the visible floor
    else if( !className.compare("FuncArea") || !className.compare("PubPoint")){
        MapEntity* parent = static_cast<MapEntity*>(mapEntity->parent());
        if(!parent->isSelected()){
            QObject* floor;
            foreach (floor, m_scene->building()->children()) {
                static_cast<MapEntity*>(floor)->setVisible(false);
            }
            parent->setVisible(true);
            m_scene->setCurrentFloor(static_cast<Floor*>(parent));
        }
        m_scene->clearSelection();
    }
    else if(!className.compare("Building")){
        m_scene->setCurrentFloor(NULL);
    }

    mapEntity->setSelected(true);
    emit selectionChanged(mapEntity);
}

Scene * DocumentView::scene() const{
    return m_scene;
}

void DocumentView::setSelectable(bool b){
    if(m_selectable != b){
        m_selectable = b;
        if(m_selectable){
            this->setDragMode(QGraphicsView::RubberBandDrag);
        }else{
            this->setDragMode(QGraphicsView::NoDrag);
        }
        m_scene->setSelectable(m_selectable);
    }
}

void DocumentView::showTexts(bool show){
    m_style ^= StyleShowShopName;
    m_scene->update();
}

DocumentView::ViewStyle DocumentView::viewStyle() {
    return m_style;
}

void DocumentView::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Control){
        m_ctrlKeyPressed = true;
    }
}

void DocumentView::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control){
        m_ctrlKeyPressed = false;
    }
}

void DocumentView::wheelEvent(QWheelEvent *event){

    if(m_ctrlKeyPressed){
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        zoom(numSteps);
    }else{
        QGraphicsView::wheelEvent(event);
    }
}

void DocumentView::zoomIn(int step){
    zoom(step);
}

void DocumentView::zoomOut(int step){
    zoom(-step);
}

void DocumentView::zoom(int step){
    m_scale += step;
    float scale = qPow(2.0, m_scale / 50.0);
    QMatrix matrix;
    matrix.scale(scale, scale);
    this->setMatrix(matrix);
}

void DocumentView::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        QGraphicsView::mousePressEvent(event);
    }
}

void DocumentView::fitView(){
    fitInView(this->sceneRect(),Qt::KeepAspectRatio);
    qreal dx = matrix().m11();
    m_scale = qLn(dx)/qLn(2.0) * 50.0;
}
