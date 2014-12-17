#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>

class DocumentView;
QT_FORWARD_DECLARE_CLASS(QGraphicsSceneMouseEvent)

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(DocumentView *parent);

    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ) = 0;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) = 0;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) = 0;
protected:
    DocumentView* m_doc;
};

#endif // ABSTRACTTOOL_H
