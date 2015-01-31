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
    Q_PROPERTY(QString brief READ brief WRITE setBrief NOTIFY briefChanged)

public:
    explicit MapEntity(QGraphicsItem *parent = 0);
    explicit MapEntity(const QString & name, QGraphicsItem *parent = 0);

    bool isClassOf(const QString &className) const;

    //setters and getters
    int id() const;
    void setId(const int id);
    const QString & enName() const;
    void setEnName(const QString & name);
    const QPointF & center() const;
    void setCenter(const QPointF & center);
    const QString & brief() const;
    void setBrief(const QString & brief);
    void setParentEntity(MapEntity* entity);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //io
    virtual bool load(const QJsonObject & jsonObject);
    virtual bool save(QJsonObject & jsonObject, double scale) const;
signals:
    void nameChanged(const QString & name);
    void enNameChanged(const QString & name);
    void idChanged(int oldId, int newId);
    void centerChanged(const QPointF & center);
    void briefChanged(const QString & brief);
protected:
    //QString m_name;
    QString m_enName;
    int m_id;
    QPointF m_center;
    QString m_brief;


};

#endif // MAPENTITY_H
