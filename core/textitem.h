#ifndef TEXTITEM_H
#define TEXTITEM_H

#include<QGraphicsTextItem>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class TextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    TextItem(QGraphicsItem *parent = 0);
    TextItem(const QString &text, QGraphicsItem *parent = 0);

signals:
    void lostFocus(TextItem *item);
    void selectedChange(QGraphicsItem *item);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // TEXTITEM_H
