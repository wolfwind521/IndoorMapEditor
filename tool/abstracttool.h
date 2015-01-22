#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>

class DocumentView;
QT_FORWARD_DECLARE_CLASS(QGraphicsSceneMouseEvent)
QT_FORWARD_DECLARE_CLASS(QKeyEvent)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QGraphicsSceneContextMenuEvent)

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(DocumentView *parent);
    virtual ~AbstractTool();
    bool isClassOf(const QString &className) const;

    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {};
    virtual void keyPressEvent(QKeyEvent *event) {};
    virtual void keyReleaseEvent(QKeyEvent *event) {};

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {};
protected:
    DocumentView* m_doc;
    QMenu* m_contextMenu;
};

#endif // ABSTRACTTOOL_H
