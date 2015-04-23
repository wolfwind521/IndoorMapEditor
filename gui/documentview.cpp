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
#include <QInputDialog>
#include <qmath.h>

#pragma execution_character_set("utf-8")

DocumentView::ViewStyle DocumentView::m_style = StyleDefault;

DocumentView::DocumentView()
    :m_isModified(false), m_selectable(true), m_ctrlKeyPressed(false), m_scale(0), m_scaleSum(0.0), m_scaleNum(0)
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
    int floorNum = m_scene->building()->floorNum();
    if(floorNum == 0)
        return;
    printer->setFromTo(1, floorNum);

    QPainter painter;
    painter.begin(printer);
    QFont font = m_scene->font();
    QFont printFont = QFont(font, painter.device());
    m_scene->setFont(printFont);
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
    m_scene->setFont(font);//restore the old font
}

void DocumentView::printCurrentView(QPrinter *printer){
    QPainter painter;
    painter.begin(printer);
    this->render(&painter);
    painter.end();
}

//selection from graphics view
void DocumentView::updateSelection(){
    if(m_scene->selectedItems().size() > 0){
        QGraphicsItem* item = m_scene->selectedItems().at(0);
        QGraphicsTextItem* textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
        if(item != NULL){
            item = item->parentItem();
        }
        MapEntity* selectedEntity = static_cast<MapEntity*>(item);
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

bool DocumentView::selectable() const{
    return m_selectable;
}

void DocumentView::showShopText(bool show){
    m_style ^= StyleShowShopName;
    m_scene->update();
}

void DocumentView::showPointText(bool show){
    m_style ^= StyleShowPointText;
    m_scene->update();
}

void DocumentView::showDirection(bool show){
    m_style ^= StyleShowDirection;
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
    fitInView(m_scene->currentFloor()->boundingRect(),Qt::KeepAspectRatio);
    qreal dx = matrix().m11();
    m_scale = qLn(dx)/qLn(2.0) * 50.0;
}

void DocumentView::addScale(double s) {
    m_scaleSum += s;
    m_scaleNum ++;
}

double DocumentView::getScale() const {
    return m_scaleNum == 0 ? 1.0 : m_scaleSum/m_scaleNum;
}

void DocumentView::onRotate(){
    bool ok;
    int angle = QInputDialog::getInt(this, tr("旋转地图"),
                                     tr("顺时针角度(0~360):"), 0, 0, 360, 1, &ok);
    if (ok){
        QMatrix mat;
        mat.rotate(angle);
        m_scene->transformMap(mat);
    }
}

void DocumentView::onFlip(){
    QString item1 = tr("水平对称(d←→b)");
    QString item2 = tr("垂直对称(甲←→由)");
    QStringList items;
    items << item1 << item2;
    bool ok;
    QString item = QInputDialog::getItem(this, tr("对称翻转"),
                                             tr("选择对称方向"), items, 0, false, &ok);
    if (ok && !item.isEmpty()){
        QMatrix mat;
        if(!item.compare(item1)){
            mat.scale(-1, 1);
        }else if(!item.compare(item2)){
            mat.scale(1, -1);
        }
        m_scene->transformMap(mat);
    }
}
