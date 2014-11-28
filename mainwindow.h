#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class DocumentView;
QT_FORWARD_DECLARE_CLASS(QTreeView)

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

    void rebuildTreeView();
private:
    bool okToContinue();
    void addDocument(DocumentView* doc);
    void setCurrentFile(const QString & fileName);
    Ui::MainWindow *ui;

    QTreeView *sceneTreeView;
    int m_maxRecentFiles;
    QStringList m_recentFiles;
    QString m_curFile;
    QString m_lastFilePath;
    DocumentView* m_docView;
};

#endif // MAINWINDOW_H
