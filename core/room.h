#ifndef ROOM_H
#define ROOM_H

#include "polygonfeature.h"
#include <QHash>
#include <QString>
#include <QJsonObject>

class QGraphicsTextItem;

class Room : public PolygonFeature
{

    Q_OBJECT
public:
    //铺位状态
    enum AreaStatus{
        Vacancy  = 0,   //空铺
        Working = 1     //正常
    };

    //铺位类别
    enum SORT_TYPE
    {
        UNSORTED = 0,       //未分类
        MIDDLE_AREA = 1,    //中岛
        SIDE_AREA = 2       //边铺
    };

    //业态
    enum Category{
        None = 0,
        Catering = 101,     //餐饮
        Shopping = 102,     //购物
        Beauty = 103,       //美妆丽人
        ParentChild = 104,  //亲子儿童
        LifeService = 105,  //生活服务
        Education = 106,    //教育培训
        LifeStyle = 107,    //生活方式
        Entertainment = 108,//休闲娱乐
        Other = 109         //其他
    };

public:
    Room(QGraphicsItem *parent = 0);
    Room(PolygonFeature &polygon);
    Room( const QString & name, const QPolygon& poly);

    bool load(const QJsonObject & jsonObject);
    bool save(QJsonObject &jsonObject) const;

    //setters and getters
    QString shopNo() const;
    void setShopNo(const QString & shopNo);

    Room::Category category() const;
    void setCategory(Room::Category cate);

    int dianpingId() const;
    void setDianpingId(int dpId);

    void setMateId(int id);
    int mateId() const;

    void setAreaStatus(Room::AreaStatus areaStatus);
    Room::AreaStatus areaStatus() const;

    void setSortType(Room::SORT_TYPE sortType);
    Room::SORT_TYPE sortType();

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
    void updateColor();
private:
    QGraphicsTextItem *m_textItem; //用于名称显示
    QString m_shopNo;       //铺位号
    Category m_category;    //业态
    int m_brandShop;    //有品牌入驻的铺位id
    int m_dianpingId;   //点评id，临时使用
    bool m_connected;   //slots have been connected
    int m_mateId;       //同铺关联id
    static QHash<QString, int> m_typeHash;
    AreaStatus m_areaStatus;    //铺位状态
    SORT_TYPE m_sortType;       //铺位类别，边铺或中岛
};

#endif // ROOM_H
