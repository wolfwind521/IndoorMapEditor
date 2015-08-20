/****************************************************************************
** ShopAnalysis
** @brief: analyze the shops in a mall
** @author: gaojie
** @date: Jul-Aug, 2015
**
****************************************************************************/
#ifndef SHOPANALYSIS_H
#define SHOPANALYSIS_H

#include "../core/building.h"
#include "../core/floor.h"
#include "../core/funcarea.h"
#include "../core/polygonentity.h"
#include <math.h>
#include <QPolygon>
#include <QString>
#include <QImage>
#include <QPainter>
#include <Qpen>
#include <QBrush>




class ShopAnalysis
{

public:
    ShopAnalysis();
    ~ShopAnalysis();
    void buildingSortArea(Building* building);

    //边铺、中岛分类
    void sideAreaSort(Floor* floor);
    //主力店分类
    void mainShopAnalysis(Floor* floor);
    //面积分类
    void acreagesort(Floor* floor);
    //形状分类
    void shapesort(Floor* floor);
    //道路提取
    //QPolygon pathextract(Floor* floor);
    void pathextract(Floor* floor);
    //点到线段的距离
    double pointToline(QPoint p0,QPoint p1,QPoint p2);
    //两多边形是否接触 默认两多边形不相交
    bool polygonTopolygon(QPolygon polygon1,QPolygon polygon2);
    //保存图片
    void save(QString fileName,QImage* outputImage);

signals:

private:



};

#endif // ShopAnalysis

