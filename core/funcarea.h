#ifndef FUNCAREA_H
#define FUNCAREA_H

#include "polygonentity.h"
#include <QHash>
#include <QString>
#include <QJsonObject>

//function area status


class QGraphicsTextItem;

class FuncArea : public PolygonEntity
{

    Q_OBJECT

    //status
    enum Status{
        Vacancy  = 0,
        Working = 1
    };

    //category
    enum Category{
        Catering = 101,
        Shopping = 102,
        Beauty = 103,
        ParentChild = 104,
        LifeService = 105,
        Education = 106,
        LifeStyle = 107,
        Entertainment = 108,
        Other = 109
    };

public:
    FuncArea(QGraphicsItem *parent = 0);
    FuncArea(PolygonEntity &polygon);
    FuncArea( const QString & name, const QPolygon& poly);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject, double scale) const;

    //setters and getters
    QString shopNo() const;
    void setShopNo(const QString & shopNo);

    FuncArea::Category category() const;
    void setCategory(FuncArea::Category cate);

    int dianpingId() const;
    void setDianpingId(int dpId);

    void setMateId(int id);
    int mateId() const;

    void setStatus(FuncArea::Status status);
    FuncArea::Status status() const;

    const QStringList typeStringList() const;
    virtual QString getTypeName();
    virtual void updateByTypeName(const QString &name);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private slots:
    void updateName(const QString &name);
    void updateCenter(const QPointF &center);
    void updateFont(const QFont &font);
private:
    QGraphicsTextItem *m_textItem;
    QString m_shopNo;
    Category m_category;
    int     m_dianpingId; //temp
    bool m_connected; //slots have been connected
    int m_mateId;     //if this id is not 0, it is the same funcarea with another one
    static QHash<QString, int> m_typeHash;
    Status m_status;
};

#endif // FUNCAREA_H
