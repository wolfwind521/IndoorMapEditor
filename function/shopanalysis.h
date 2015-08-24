/****************************************************************************
** ShopAnalysis
** @brief: analyze the shops in a mall
** @author: gaojie
** @date: Jul-Aug, 2015
**
****************************************************************************/
#ifndef SHOPANALYSIS_H
#define SHOPANALYSIS_H


#include <math.h>
#include <QPolygon>


class ShopAnalysis
{

public:
    ShopAnalysis();
    ~ShopAnalysis();
    void buildingSortArea(Building* building);

    //边铺、中岛分类
    void sideAreaSort(Floor* floor);

    //点到线段的距离
    double pointToline(const QPoint &p0, const QPoint &p1, const QPoint &p2);
    //两多边形是否接触 默认两多边形不相交
    bool polygonTopolygon(QPolygon &polygon1,QPolygon &polygon2);


signals:

private:



};

#endif // ShopAnalysis

