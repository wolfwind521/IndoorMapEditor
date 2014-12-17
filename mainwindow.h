#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class DocumentView;
class PropertyView;

QT_FORWARD_DECLARE_CLASS(QTreeView)
QT_FORWARD_DECLARE_CLASS(QPrinter)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DocumentView *currentDocument() const;
public slots:
    void newDocument();
    void openDocument();
    bool saveDocument();
    void saveAsDocument();
    void closeDocument();
    void exportDocument();
    void printDocument();

    void rebuildTreeView();

    void setPolygonTool();
    void setSelectTool();
    void setPubPointTool();
private:
    bool okToContinue();
    void addDocument(DocumentView* doc);
    void setCurrentFile(const QString & fileName);
    Ui::MainWindow *ui;

    QTreeView *m_sceneTreeView;
    PropertyView *m_propertyView;
    int m_maxRecentFiles;
    QStringList m_recentFiles;
    QString m_curFile;
    QString m_lastFilePath;
    DocumentView* m_docView;
    QPrinter *m_printer;
};

#endif // MAINWINDOW_H
