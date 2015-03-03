#include "textitem.h"

TextItem::TextItem(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

TextItem::TextItem(QGraphicsItem *parent)
    :QGraphicsTextItem(parent){
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant TextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void TextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
