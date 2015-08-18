#ifndef SHOPANALYSIS_H
#define SHOPANALYSIS_H

/***************************************************
 ** Building
 ** @brief: the class for shop analysis
 ** @author: gaojie, gaimeng
 ** @date: Aug, 2015
 **
 ****************************************************/

class ShopAnalysis
{
public:
    ShopAnalysis();
    ~ShopAnalysis();

    //点到线段的距离
    double pointToLine(QPoint p0,QPoint p1,QPoint p2);

    //两多边形是否接触 默认两多边形不相交
    bool polygonTopolygon(QPolygon polygon1, QPolygon polygon2);


};

#endif // SHOPANALYSIS_H
