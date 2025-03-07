#ifndef MAPENTITY_H
#define MAPENTITY_H

#include <QGraphicsObject>
#include <QJsonObject>
#include<QPointF>

//base class for all the entities in the map.
class Feature : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString enName READ enName WRITE setEnName NOTIFY enNameChanged)
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(QString brief READ brief WRITE setBrief NOTIFY briefChanged)

public:
    explicit Feature(QGraphicsItem *parent = 0);
    explicit Feature(const QString & name, QGraphicsItem *parent = 0);

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
    const QString & type();
    void setType(const QString & type);

    virtual const QStringList typeStringList() const{return QStringList();}
    virtual QString getTypeName(){return QString();}
    virtual void updateByTypeName(const QString &typeName){}

    void setParentFeature(Feature* feature);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //io
    virtual bool load(const QJsonObject & jsonObject);
    virtual bool save(QJsonObject & jsonObject) const;

    virtual void transformFeature(const QTransform &matrix);
    virtual int generateId();
signals:
    void nameChanged(const QString & name);
    void enNameChanged(const QString & name);
    void idChanged(int oldId, int newId);
    void centerChanged(const QPointF & center);
    void briefChanged(const QString & brief);
    void typeChanged(const QString & type);
protected:

    QString m_enName;
    int     m_id;
    QPointF m_center;
    QString m_brief;
    QString m_type;

};

#endif // MAPENTITY_H
