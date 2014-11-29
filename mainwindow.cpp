#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/documentview.h"
#include "gui/scenemodel.h"
#include "core/building.h"
#include "io/iomanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_maxRecentFiles(5),
    m_lastFilePath(".")
{
    ui->setupUi(this);

    sceneTreeView = new QTreeView(ui->dockTreeWidget);
    ui->dockTreeWidget->setWidget(sceneTreeView);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openDocument()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveDocument()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeDocument()));

    addDocument(new DocumentView());
    setCurrentFile("");

    connect(sceneTreeView, SIGNAL(clicked(QModelIndex)), m_docView, SLOT(selectEntity(QModelIndex)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

DocumentView *MainWindow::currentDocument() const
{
    return m_docView;
}

void MainWindow::openDocument()
{
    if(okToContinue()){
        //TODO: vector graphic file support
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("打开文件"), m_lastFilePath,
                                                        tr("Json文件 (*.json)\n"
                                                           "图像文件 (*.jpg *.png *.bmp)"));
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

void MainWindow::addDocument(DocumentView *doc){
    //TODO: connect the slots
    m_docView = doc;
    this->setCentralWidget(doc);
}

void MainWindow::newDocument()
{
    if(okToContinue()){
        currentDocument()->clear();
        setCurrentFile("");

        rebuildTreeView();
    }
}

bool MainWindow::saveDocument()
{
    return true;
}

void MainWindow::saveAsDocument()
{

}

void MainWindow::closeDocument()
{

}

void MainWindow::exportDocument()
{

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
            return saveDocument();
        }else if(r == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

void MainWindow::rebuildTreeView(){
    SceneModel *model = new SceneModel(m_docView->root());
    sceneTreeView->setModel(model);
    sceneTreeView->expandAll();
//        ObjectTreeModel *model = new ObjectTreeModel(this);
//        sceneTreeView->setModel(model);
}
