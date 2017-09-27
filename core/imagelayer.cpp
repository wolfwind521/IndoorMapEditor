#include "imagelayer.h"
#include <QFileInfo>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>

ImageLayer::ImageLayer(QGraphicsItem *parent) :
    Feature(parent), m_image(NULL)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemStacksBehindParent);
}

ImageLayer::~ImageLayer(){
    if(m_image){
        delete m_image;
        m_image = NULL;
    }
}

bool ImageLayer::setImage(const QString &fileName){
    m_fileName = fileName;
    if(m_image){
        m_image->load(fileName);
    }else{
        m_image = new QPixmap(fileName);
    }
    setObjectName(QFileInfo(fileName).fileName());
    setCenter(QPointF(- m_image->width() / 2.0, - m_image->height() / 2.0));
    return !(m_image->isNull());
}

QRectF ImageLayer::boundingRect() const {
    return m_image->rect().translated(center().toPoint());
}

void ImageLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPixmap(center(), *m_image);
    if(option->state & QStyle::State_Selected){
        QColor color = QColor(0, 160, 233);
        painter->setPen(QPen(color, 2));
        painter->drawRect(m_image->rect().translated(center().toPoint()));
    }
}

bool ImageLayer::load(const QJsonObject &jsonObject) {
    Feature::load(jsonObject);
    setImage(jsonObject["file"].toString());
    return true;
}

bool ImageLayer::save(QJsonObject &jsonObject) const {
    Feature::save(jsonObject);
    jsonObject["file"] = m_fileName;
    return true;
}
