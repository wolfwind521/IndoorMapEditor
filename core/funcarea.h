#ifndef FUNCAREA_H
#define FUNCAREA_H

#include "polygonentity.h"

#include <QString>
#include <QJsonObject>

enum FUNC_TYPE
{
    CHINESE_FOOD,
    //TODO: more types
};

class FuncArea : public PolygonEntity
{

    Q_OBJECT
    Q_PROPERTY(FUNC_TYPE funcType READ funcType WRITE setFuncType NOTIFY funcTypeChanged)
public:
    FuncArea(QGraphicsItem *parent = 0);

    bool load(const QJsonObject & jsonObject);

    //setters and getters
    FUNC_TYPE funcType() const;
    void setFuncType(const FUNC_TYPE type);
signals:
    void funcTypeChanged(FUNC_TYPE type);
private:

    FUNC_TYPE m_type;
};

#endif // FUNCAREA_H
