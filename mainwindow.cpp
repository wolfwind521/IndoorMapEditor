#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./gui/documentview.h"
#include "./gui/propertyview.h"
#include "./gui/propviewfuncarea.h"
#include "./gui/scenemodel.h"
#include "./core/building.h"
#include "./core/scene.h"
#include "./io/iomanager.h"
#include "./tool/toolmanager.h"
#include "./tool/polygontool.h"
#include "./tool/selecttool.h"
#include "./tool/pubpointtool.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#ifndef QT_NO_PRINTER
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_maxRecentFiles(5),
    m_lastFilePath("."),
    m_printer(NULL)
{
    ui->setupUi(this);

    m_sceneTreeView = new QTreeView(ui->dockTreeWidget);
    ui->dockTreeWidget->setWidget(m_sceneTreeView);
    m_propertyView = new PropertyView(ui->dockPropertyWidget);
    ui->dockPropertyWidget->setWidget(m_propertyView);

    QActionGroup * toolActionGroup = new QActionGroup(this);
    toolActionGroup->addAction(ui->actionSelectTool);
    toolActionGroup->addAction(ui->actionPolygonTool);
    toolActionGroup->addAction(ui->actionPubPointTool);

    //menus action
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAsFile()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteEntity()));

    //tools action
    connect(ui->actionPolygonTool, SIGNAL(triggered()), this, SLOT(setPolygonTool()));
    connect(ui->actionSelectTool, SIGNAL(triggered()), this, SLOT(setSelectTool()));
    connect(ui->actionPubPointTool, SIGNAL(triggered()), this, SLOT(setPubPointTool()));

    addDocument(new DocumentView());
    setCurrentFile("");
    rebuildTreeView();

    ToolManager::instance()->setTool(new SelectTool(currentDocument()));

    connect(m_sceneTreeView, SIGNAL(clicked(QModelIndex)), m_docView, SLOT(updateSelection(QModelIndex)));
    connect(m_docView, SIGNAL(selectionChanged(MapEntity*)), this, SLOT(updatePropertyView(MapEntity*)));
    connect(m_docView->scene(), SIGNAL(buildingChanged()), this, SLOT(rebuildTreeView()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
                                                        tr("Json文件 (*.json)\n"
                                                           "图像文件 (*.jpg *.jpeg *.png *.bmp *.gif)"));
        if(fileName.isEmpty())
            return;

        m_lastFilePath = QFileInfo(fileName).absoluteFilePath();//save the last path

        if(IOManager::loadFile(fileName, currentDocument()))
        {
            statusBar()->showMessage(tr("文件载入成功"), 2000);
            setCurrentFile(fileName);

            rebuildTreeView(); //rebuild the treeView
        }else{
            QMessageBox::warning(this,
                                tr("Parse error"),
                                tr("文件载入失败\n%1").arg(fileName));
            return;
        }
    }
}

void MainWindow::addDocument(DocumentView *doc) {
    //TODO: connect the slots
    m_docView = doc;
    this->setCentralWidget(doc);
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

    saveDocument(fileName);
}

void MainWindow::closeFile()
{

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

void MainWindow::deleteEntity(){
    currentDocument()->scene()->deleteSelected();
}

void MainWindow::setCurrentFile(const QString & fileName){
    m_curFile = fileName;
    currentDocument()->setModified(false);

    QString shownName = tr("Untitle");
    if(! m_curFile.isEmpty()){
        shownName = QFileInfo(fileName).fileName();
        m_recentFiles.removeAll(m_curFile);
        m_recentFiles.prepend(m_curFile);
        //TODO: update actions
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("IndoorMap Editor")));
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

void MainWindow::updatePropertyView(MapEntity *mapEntity) {
    if(mapEntity == NULL)
        return;
    QString className = mapEntity->metaObject()->className();
    if(!m_propertyView->match(mapEntity)){
        delete m_propertyView;
        //ugly codes. should be replaced by a factory class later.
        if(className == "FuncArea"){
            m_propertyView = new PropViewFuncArea(ui->dockPropertyWidget);
        }else {
            m_propertyView = new PropertyView(ui->dockPropertyWidget);
        }
        ui->dockPropertyWidget->setWidget(m_propertyView);
    }
    m_propertyView->setMapEntity(mapEntity);
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
