#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./gui/documentview.h"
#include "./gui/propertyview.h"
#include "./gui/propviewroom.h"
#include "./gui/propviewbuilding.h"
#include "./gui/propviewfloor.h"
#include "./gui/scenemodel.h"
#include "./core/building.h"
#include "./core/scene.h"
#include "./io/iomanager.h"
#include "./tool/toolmanager.h"
#include "./tool/polygontool.h"
#include "./tool/selecttool.h"
#include "./tool/pubpointtool.h"
#include "./tool/mergetool.h"
#include "./tool/splittool.h"
#include "./tool/scaletool.h"
#include <time.h>
#include <QTimer>
#include <QCloseEvent>
#include <QSettings>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QListWidgetItem>
#include <QListWidget>
#include <QInputDialog>
#include <QMimeData>
#ifndef QT_NO_PRINTER
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif
#include <QActionGroup>


#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_maxRecentFiles(5),
    m_lastFilePath("."),
    m_printer(NULL),
    m_propertyView(NULL),
    m_timer(NULL),
    m_searchResultIter(m_searchResults)
{
    ui->setupUi(this);
    // srand(time(0));
    arc4random();

    m_sceneTreeView = new QTreeView(ui->dockTreeWidget);
    ui->verticalLayout->insertWidget(0,m_sceneTreeView);
    addDocument(new DocumentView());
    setCurrentFile("");
    rebuildTreeView();

    ToolManager::instance()->setTool(new SelectTool(currentDocument()));


    QActionGroup * toolActionGroup = new QActionGroup(this);
    toolActionGroup->addAction(ui->actionSelectTool);
    toolActionGroup->addAction(ui->actionPolygonTool);
    toolActionGroup->addAction(ui->actionPubPointTool);
    toolActionGroup->addAction(ui->actionMergeTool);
    toolActionGroup->addAction(ui->actionSplitTool);
    toolActionGroup->addAction(ui->actionScaleTool);

    //recent file actions
    QMenu *recentFileMenu = new QMenu(this);
    m_recentFileActions.resize(m_maxRecentFiles);
    for(int i = 0; i < m_recentFileActions.size(); i++){
        m_recentFileActions[i]= new QAction(this);
        m_recentFileActions[i]->setVisible(false);
        connect(m_recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        recentFileMenu->addAction(m_recentFileActions[i]);
    }
    ui->actionRecent->setMenu(recentFileMenu);
    //menus action
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAsFile()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
    connect(ui->actionPrintCurrent, SIGNAL(triggered()), this, SLOT(printCurrent()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteFeature()));
    connect(ui->actionFont, SIGNAL(triggered()), this, SLOT(setGraphicsViewFont()));
    connect(ui->actionFindRepeat, SIGNAL(triggered()), this, SLOT(findAllRepeat()));
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(onFind()));
    connect(ui->actionSortAreas, SIGNAL(triggered()), this, SLOT(sortAreas()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), m_docView, SLOT(zoomOut()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), m_docView, SLOT(zoomIn()));
    connect(ui->actionResetZoom, SIGNAL(triggered()), m_docView, SLOT(fitView()));
    connect(ui->actionRotate, SIGNAL(triggered()), m_docView,  SLOT(onRotate()));
    connect(ui->actionFlip, SIGNAL(triggered()), m_docView, SLOT(onFlip()));
    connect(ui->actionShowShopText, SIGNAL(toggled(bool)), m_docView, SLOT(showShopText(bool)));
    connect(ui->actionShowPointText, SIGNAL(toggled(bool)), m_docView, SLOT(showPointText(bool)));
    connect(ui->actionShowDir, SIGNAL(toggled(bool)), m_docView, SLOT(showDirection(bool)));
    connect(ui->actionCut, SIGNAL(triggered()), m_docView, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), m_docView, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), m_docView, SLOT(paste()));

    //tools action
    connect(ui->actionPolygonTool, SIGNAL(triggered()), this, SLOT(setPolygonTool()));
    connect(ui->actionSelectTool, SIGNAL(triggered()), this, SLOT(setSelectTool()));
    connect(ui->actionPubPointTool, SIGNAL(triggered()), this, SLOT(setPubPointTool()));
    connect(ui->actionMergeTool, SIGNAL(triggered()), this, SLOT(setMergeTool()));
    connect(ui->actionSplitTool, SIGNAL(triggered()), this, SLOT(setSplitTool()));
    connect(ui->actionScaleTool, SIGNAL(triggered()), this, SLOT(setScaleTool()));


    //other actions
    connect(m_sceneTreeView, SIGNAL(clicked(QModelIndex)), m_docView, SLOT(updateSelection(QModelIndex)));
    connect(m_docView, SIGNAL(selectionChanged(Feature*)), this, SLOT(updatePropertyView(Feature*)));
    connect(m_docView->scene(), SIGNAL(buildingChanged()), this, SLOT(rebuildTreeView()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));
    connect(ui->addLayerButton, SIGNAL(clicked()), this, SLOT(onAddLayerButtonClicked()));
    //connect(ui->actionShowAreaSort, SIGNAL(toggled(bool)), m_docView, SLOT(showAreaSort(bool)));


    m_docView->scene()->setFont(QFont(tr("微软雅黑"), 26));

    //autosave
    m_autoSaveTime = 300000; //autosave every 5min
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(autoSave()) );
    m_timer->start(m_autoSaveTime);

    setAcceptDrops(true);
    readSettings();
    QSettings settings("Wolfwind", "IndoorMapEditor");
    settings.setValue("normallyClosed", false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(okToContinue()){
        writeSettings();
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event){
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return;
    openDocument(fileName);
}

DocumentView *MainWindow::currentDocument() const
{
    return m_docView;
}

void MainWindow::openFile()
{
    if(okToContinue()){
        //TODO: vector graphic file support
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("打开文件"), m_lastFilePath,
                                                        tr("全部文件 (*.json *.jpg *.jpeg *.png *.bmp *.gif)\n"
                                                            "Json文件 (*.json)\n"
                                                           "图像文件 (*.jpg *.jpeg *.png *.bmp *.gif)"));
        if(!fileName.isEmpty())
            openDocument(fileName);
    }
}

void MainWindow::openRecentFile(){
    if(okToContinue()){
        QAction *action = qobject_cast<QAction *>(sender());
        if(action){
            openDocument(action->data().toString());
        }
    }
}

void MainWindow::addDocument(DocumentView *doc) {
    //TODO: connect the slots
    m_docView = doc;
    this->setCentralWidget(doc);
}

void MainWindow::openDocument(const QString &fileName){
    m_lastFilePath = QFileInfo(fileName).absoluteFilePath();//save the last path
    currentDocument()->clear();
    if(IOManager::loadFile(fileName, currentDocument()))
    {
         rebuildTreeView(); //rebuild the treeView
        statusBar()->showMessage(tr("文件载入成功"), 2000);
        if(QFileInfo(fileName).suffix() == "json"){
            setCurrentFile(fileName);
            autoSave();
            currentDocument()->scene()->showDefaultFloor();
            currentDocument()->fitView();
        }


    }else{
        QMessageBox::warning(this,
                            tr("Parse error"),
                            tr("文件载入失败\n%1").arg(fileName));
        return;
    }
}

bool MainWindow::saveDocument(const QString &fileName){
    if(IOManager::saveFile(fileName, currentDocument())){
        statusBar()->showMessage(tr("文件保存成功"), 2000);
        setCurrentFile(fileName);
        return true;
    }else{
        statusBar()->showMessage(tr("文件保存失败"), 2000);
        return false;
    }
}

void MainWindow::newFile() {
    if(okToContinue()){
        currentDocument()->clear();
        setCurrentFile("");

        rebuildTreeView();
    }
}

bool MainWindow::saveFile() {
    if(m_curFile.isEmpty()){
        return saveAsFile();
    }else{
        return saveDocument(m_curFile);
    }
}

bool MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Files"),".",tr("Indoor map files(*.json)"));
    if(fileName.isEmpty()){
        return false;
    }

    return saveDocument(fileName);
}

void MainWindow::autoSave(){
    QString tmpDir = QDir::tempPath();
    IOManager::saveFile(tmpDir + "/autoSaveFile.json", currentDocument());
}

void MainWindow::closeFile()
{
    if(okToContinue()){
        currentDocument()->clear();
        setCurrentFile("");

        rebuildTreeView();
    }
}

void MainWindow::exportFile()
{

}

void MainWindow::printFile()
{
    if(m_printer == NULL)
        m_printer = new QPrinter(QPrinter::HighResolution);

    if(!m_printer->isValid()){
        QMessageBox::warning(this, tr("Error"),tr("No printer found"),QMessageBox::Ok);
        return;
    }

    QPrintPreviewDialog preview(m_printer, this);
    //m_printer->setOutputFormat(QPrinter::NativeFormat);

    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
             currentDocument(), SLOT(printScene(QPrinter*)));

    preview.exec();

//    QPrintDialog printDialog(m_printer, this);
//    if(printDialog.exec() == QDialog::Accepted){
//        QPainter painter(m_printer);
//        currentDocument()->printScene(&painter);
//        statusBar()->showMessage(tr("Printed %1").arg(windowFilePath()), 2000);
//    }
}

void MainWindow::printCurrent(){
    if(m_printer == NULL)
        m_printer = new QPrinter(QPrinter::HighResolution);

    if(!m_printer->isValid()){
        QMessageBox::warning(this, tr("Error"),tr("No printer found"),QMessageBox::Ok);
        return;
    }

    QPrintPreviewDialog preview(m_printer, this);

    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
             currentDocument(), SLOT(printCurrentView(QPrinter*)));

    preview.exec();
}

void MainWindow::deleteFeature(){

    currentDocument()->scene()->deleteSelectedItems();
    rebuildTreeView();
}

void MainWindow::onDeleteButtonClicked(){
    int r = QMessageBox::warning(this, tr("Warning"),
                                 tr("确定删除？"),
                                 QMessageBox::Yes | QMessageBox::No );
    if(r == QMessageBox::Yes ){
        currentDocument()->scene()->deleteSelectedLayers();
        rebuildTreeView();
    }
}

void MainWindow::onAddLayerButtonClicked(){
    currentDocument()->scene()->addFloor();
}

void MainWindow::setCurrentFile(const QString & fileName){
    m_curFile = fileName;
    currentDocument()->setModified(false);

    QString shownName = tr("Untitle");
    if(! m_curFile.isEmpty()){
        shownName = QFileInfo(fileName).fileName();
        m_recentFiles.removeAll(m_curFile);
        m_recentFiles.prepend(m_curFile);
        updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("IndoorMap Editor")));
}

void MainWindow::readSettings(){
    QSettings settings("Wolfwind", "IndoorMapEditor");

    bool normallyClosed = settings.value("normallyClosed", false).toBool();
    if(!normallyClosed){
        QString path = QDir::tempPath();
        QFileInfo fileInfo(path + "/autoSaveFile.json");
        if(fileInfo.exists()){
            int r = QMessageBox::warning(this, tr("Warning"),
                                         tr("发现意外关闭时的缓存文件，是否恢复？<b><font color=red>若您之前已保存了所需文件，请选择“否”并手动打开之前保存的文件。</font></b>"),
                                         QMessageBox::Yes | QMessageBox::No );
            if(r == QMessageBox::Yes){
                //open the autosave file
                openDocument(fileInfo.absoluteFilePath());
                m_curFile.clear();
            }
        }
    }

    m_recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
}

void MainWindow::writeSettings(){
    QSettings settings("Wolfwind", "IndoorMapEditor");
    settings.setValue("recentFiles", m_recentFiles);
    settings.setValue("normallyClosed", true);
}

void MainWindow::updateRecentFileActions(){
    QMutableStringListIterator i(m_recentFiles);
    while(i.hasNext()){
        if(!QFile::exists(i.next()))
            i.remove();
    }
    for(int j = 0; j < m_recentFileActions.size(); j++){
        if(j < m_recentFiles.count()){
            QString text = tr("&%1 %2").arg(j+1).arg( QFileInfo(m_recentFiles[j]).fileName());
            m_recentFileActions[j]->setText(text);
            m_recentFileActions[j]->setData(m_recentFiles[j]);
            m_recentFileActions[j]->setVisible(true);
        }else{
            m_recentFileActions[j]->setVisible(false);
        }
    }
}

bool MainWindow::okToContinue(){
    if(currentDocument()->isModified()){
        int r = QMessageBox::warning(this, tr("Warning"),
                                     tr("the file has been modifed\n"
                                     "do you want to save? "),
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(r == QMessageBox::Yes){
            return saveFile();
        }else if(r == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

void MainWindow::rebuildTreeView(){
    SceneModel *model = new SceneModel(m_docView->scene()->root());
    m_sceneTreeView->setModel(model);
    m_sceneTreeView->expandToDepth(0);
}

void MainWindow::updatePropertyView(Feature *mapFeature) {
    if(mapFeature == NULL && m_propertyView!=NULL){
        delete m_propertyView;
        m_propertyView = NULL;
        return;
    }
    QString className = mapFeature->metaObject()->className();

    if(m_propertyView== NULL || !m_propertyView->match(mapFeature)){
        if(m_propertyView != NULL)
            delete m_propertyView;
        //ugly codes. should be replaced by a factory class later.
        if(className == "Room"){
            m_propertyView = new PropViewRoom(mapFeature, ui->dockPropertyWidget);
        }else if(className == "Building"){
            m_propertyView = new PropViewBuilding(mapFeature, ui->dockPropertyWidget);
        }else if(className == "Floor"){
            m_propertyView = new PropViewFloor(mapFeature, ui->dockPropertyWidget);
        }else{
            m_propertyView = new PropertyView(mapFeature, ui->dockPropertyWidget);
        }
        ui->dockPropertyWidget->setWidget(m_propertyView);
    }else{
        m_propertyView->setMapFeature(mapFeature);
    }
}

void MainWindow::setPolygonTool(){
    AbstractTool *tool = new PolygonTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(false);
}

void MainWindow::setSelectTool(){
    AbstractTool *tool = new SelectTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(true);
}

void MainWindow::setPubPointTool(){
    AbstractTool *tool = new PubPointTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(false);
}

void MainWindow::setMergeTool(){
    AbstractTool *tool = new MergeTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(true);
}

void MainWindow::setSplitTool(){
    AbstractTool *tool = new SplitTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(false);
}

void MainWindow::setScaleTool(){
    AbstractTool *tool = new ScaleTool(currentDocument());
    ToolManager::instance()->setTool(tool);
    currentDocument()->setSelectable(false);
}

void MainWindow::setGraphicsViewFont(){
    bool ok;
    //QFontDialog::setCurrentFont(QApplication::font("DocumentView"));
    QGraphicsScene *scene = currentDocument()->scene();
    QFont font = QFontDialog::getFont(&ok, scene->font(), this );
    if ( ok ) {
          scene->setFont(font);
    }
}

void MainWindow::onFind(){
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("搜索"),
                                             tr("名称:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !searchText.isEmpty()){
        m_searchResults = currentDocument()->scene()->findMapFeature(searchText);

        if(m_searchResults.isEmpty()){
            QMessageBox::warning(this, tr("搜索结果"),(QString("未找到 ")+searchText),QMessageBox::Ok);
            return;
        }else{
            QListWidget *listWidget = new QListWidget(this);
            foreach(Feature *mapFeature, m_searchResults){
                QListWidgetItem * listItem = new QListWidgetItem(mapFeature->objectName()+" @ "+mapFeature->parent()->objectName(), listWidget);
                listItem->setData(Qt::UserRole, QVariant::fromValue(mapFeature));
            }
            connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(outputItemClicked(QListWidgetItem*)));

            ui->dockOutputWidget->setWidget(listWidget);
            ui->dockOutputWidget->setVisible(true);
        }
        currentDocument()->scene()->selectMapFeature((m_searchResultIter.next()));
    }
}

void MainWindow::findAllRepeat(){
    QListWidget *listWidget = new QListWidget(this);

    const QList<QList<Feature*> > & result = currentDocument()->scene()->findAllRepeat();
    foreach(const QList<Feature*> & list, result){
        Feature *mapFeature;
        foreach(mapFeature, list){
            QListWidgetItem * listItem = new QListWidgetItem(mapFeature->objectName()+" @ "+mapFeature->parent()->objectName(), listWidget);
            listItem->setData(Qt::UserRole, QVariant::fromValue(mapFeature));
        }
    }

    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(outputItemClicked(QListWidgetItem*)));

    ui->dockOutputWidget->setWidget(listWidget);
    ui->dockOutputWidget->setVisible(true);
}

void MainWindow::outputItemClicked(QListWidgetItem* item){
    Feature* mapFeature = item->data(Qt::UserRole).value<Feature*>();
    if(mapFeature != NULL){
        currentDocument()->scene()->selectMapFeature(mapFeature);
    }
}

void MainWindow::sortAreas(){

}
