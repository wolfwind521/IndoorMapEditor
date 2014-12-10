#ifndef MAPENTITY_H
#define MAPENTITY_H

#include <QGraphicsObject>
#include <QJsonObject>

class MapEntity : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString enName READ enName WRITE setEnName NOTIFY enNameChanged)
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
public:
    explicit MapEntity(QGraphicsItem *parent = 0);
    explicit MapEntity(const QString & name, QGraphicsItem *parent = 0);

    //setters and getters
//    QString name() const;
//    void setName(const QString & name);
    int id() const;
    void setId(const int id);
    const QString & enName() const;
    void setEnName(const QString & name);
    const QPointF & center() const;
    void setCenter(const QPointF & center);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //io
    virtual bool load(const QJsonObject & jsonObject);
    virtual bool save(QJsonObject & jsonObject);
signals:
    void nameChanged(const QString & name);
    void enNameChanged(const QString & name);
    void idChanged(int id);
    void centerChanged(const QPointF & center);
protected:
    //QString m_name;
    QString m_enName;
    int m_id;
    QPointF m_center;


};

#endif // MAPENTITY_H
