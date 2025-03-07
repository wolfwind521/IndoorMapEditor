#include "documentview.h"
#include "../core/building.h"
#include "../core/feature.h"
#include "../core/floor.h"
#include "../core/room.h"
#include "../core/pubpoint.h"
#include "../core/scene.h"
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QUndoStack>
#include <QKeyEvent>
#include <QtPrintSupport/QPrinter>
#include <QApplication>
#include <QInputDialog>
#include <QMimeData>
#include <QClipboard>
#include <QJsonDocument>
#include <qmath.h>

#pragma execution_character_set("utf-8")

DocumentView::ViewStyle DocumentView::m_style = StyleDefault;

DocumentView::DocumentView()
    :m_isModified(false), m_selectable(true), m_ctrlKeyPressed(false), m_zoom(0)
{
    m_scene = new Scene(this);
    m_scene->reset();
    m_undoStack = new QUndoStack(this);
    setAcceptDrops(false);
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

void DocumentView::cut(){

}

void DocumentView::copy(){
    QJsonObject rootObject;
    QJsonArray floorArray;
    QJsonArray funcAreaArray;

    QList<QGraphicsItem*> itemList = m_scene->selectedItems();
    foreach(QGraphicsItem* item, itemList) {
        Feature *mapFeature = dynamic_cast<Feature*>(item);
        if(mapFeature == NULL)
            continue;
        QJsonObject jsonOject;
        mapFeature->save(jsonOject);
        if(mapFeature->isClassOf("Floor")){
            jsonOject.remove("FuncAreas");
            jsonOject.remove("PubPoint");
            jsonOject.remove("ImageLayer");
            floorArray.append(jsonOject);
        }else if(mapFeature->isClassOf("Room")){
            funcAreaArray.append(jsonOject);
        }
    }
    if(!funcAreaArray.isEmpty())
        rootObject["FuncAreas"] = funcAreaArray;
    if(!floorArray.isEmpty())
        rootObject["Floors"] = floorArray;
    QJsonDocument jsonDoc(rootObject);
    QApplication::clipboard()->setText(QString(jsonDoc.toJson()));
}

void DocumentView::paste(){
    QString text= QApplication::clipboard()->text();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(text.toUtf8()));
    QJsonObject rootObject = jsonDoc.object();
    Floor *newFloor = NULL;
    if(rootObject.contains("Floors")){
        QJsonArray floorsArray = rootObject["Floors"].toArray();
        for(int i = 0; i < floorsArray.size(); i++){
            QJsonObject floorObject = floorsArray[i].toObject();
            newFloor = new Floor(m_scene->building());
            newFloor->load(floorObject);
            newFloor->generateId();
            m_scene->addFloor(newFloor);
        }

    }
    if(newFloor == NULL)
        newFloor = m_scene->currentFloor();
    if(newFloor == NULL)
        newFloor = m_scene->building()->getFloors().back();
    if(rootObject.contains("FuncAreas")){
        QJsonArray funcArray = rootObject["FuncAreas"].toArray();
        for(int i = 0; i < funcArray.size(); i++){
            QJsonObject funcObject = funcArray[i].toObject();
            Room* newFuncArea = new Room(newFloor);
            newFuncArea->load(funcObject);
            newFuncArea->generateId();
            m_scene->addRoom(newFuncArea);
        }
    }
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
        // m_scene->render(&painter, printer->pageRect(), newRect, Qt::KeepAspectRatio);
        m_scene->render(&painter, printer->pageRect(QPrinter::Millimeter), newRect, Qt::KeepAspectRatio);
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
    QList<QGraphicsItem*> selectedItems = m_scene->selectedItems();
    QGraphicsItem *mapFeature;
    if(selectedItems.size() > 0){
        //textItems
        QList<QGraphicsItem*>::iterator iter;
        for(iter = selectedItems.begin(); iter != selectedItems.end(); iter++){
            if(qgraphicsitem_cast<QGraphicsTextItem*>(*iter)){
                QGraphicsItem *parent = (*iter)->parentItem();
                parent->setSelected(true);
                mapFeature = parent;
            }else
                mapFeature = *iter;
        }

        Feature* selectedFeature = static_cast<Feature*>(mapFeature);
        m_scene->clearSelectedLayers();
        m_scene->setSelectedLayer(m_scene->currentFloor());
        emit selectionChanged(selectedFeature);
    }else{
        emit selectionChanged(NULL);
    }
}

//selection from tree view
void DocumentView::updateSelection(const QModelIndex & index){
    Feature *mapFeature = static_cast<Feature*>(index.internalPointer());


    QString className = mapFeature->metaObject()->className();
    //a floor selected, change the visible floor
    if(!className.compare("Building")){
        QObject *floorObject;
        foreach (floorObject, m_scene->building()->children()) {
            static_cast<Feature*>(floorObject)->setVisible(false);
        }
        m_scene->setCurrentFloor(NULL);
    }
    //a floor selected, change the visible floor
    else if( !className.compare("Floor")){
        QObject *floorObject;
        foreach (floorObject, m_scene->building()->children()) {
            static_cast<Feature*>(floorObject)->setVisible(false);
        }
        mapFeature->setVisible(true);
        Floor *floor = static_cast<Floor*>(mapFeature);
        m_scene->setCurrentFloor(floor);
    }
    //a room or pubPoint selected, change the visible floor
   // else if( !className.compare("Room") || !className.compare("PubPoint") || !className.compare("ImageLayer")){
    else{
        Feature* parent = static_cast<Feature*>(mapFeature->parent());
        if(!parent->isSelected()){
            QObject* floor;
            foreach (floor, m_scene->building()->children()) {
                static_cast<Feature*>(floor)->setVisible(false);
            }
            parent->setVisible(true);
            m_scene->setCurrentFloor(static_cast<Floor*>(parent));
        }
        m_scene->clearSelection();
        mapFeature->setSelected(true);
    }
    m_scene->setSelectedLayer(mapFeature);
    this->update();

    emit selectionChanged(mapFeature);
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

void DocumentView::showAreaSort(bool show){
    m_style ^= StyleShowAreaSort;
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
        // int numDegrees = event->delta() / 8;
        int numDegrees = event->angleDelta().y() / 8;
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
    m_zoom += step;
    float scale = qPow(2.0, m_zoom / 50.0);
    QTransform matrix;
    matrix.scale(scale, scale);
    // this->setMatrix(matrix);
    this->setTransform(matrix);
}

void DocumentView::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        QGraphicsView::mousePressEvent(event);
    }
}

void DocumentView::fitView(){
    fitInView(m_scene->currentFloor()->boundingRect(),Qt::KeepAspectRatio);
    qreal dx = transform().m11();
    m_zoom = qLn(dx)/qLn(2.0) * 50.0;
}


void DocumentView::onRotate(){
    bool ok;
    int angle = QInputDialog::getInt(this, tr("旋转地图"),
                                     tr("顺时针角度(0~360):"), 0, 0, 360, 1, &ok);
    if (ok){
        QTransform mat;
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
        QTransform mat;
        if(!item.compare(item1)){
            mat.scale(-1, 1);
        }else if(!item.compare(item2)){
            mat.scale(1, -1);
        }
        m_scene->transformMap(mat);
    }
}
