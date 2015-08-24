#include "../core/building.h"
#include "../core/floor.h"
#include "../core/funcarea.h"
#include "../core/polygonentity.h"
#include "shopanalysis.h"
#include <QString>
#include <QImage>
#include <QPainter>
#include <Qpen>
#include <QBrush>




//边铺中岛区分
double floorthreshold=5;
double areathreshold=5;
double ratio=0.05;
double threshold=5;
//面积区分
double acreageAthreshold=1000;
double acreageBthreshold=500;
double acreageCthreshold=150;
double acreageDthreshold=50;
//形状区分
double acreagedifference=0.3;
double lengthWidthAratio=1.3;
double lengthWidthBratio=4;
//路径提取
/*
int array[255]={0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,
                1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,
                0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,
                1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,
                1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,
                1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,
                0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,1,
                1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
                1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
                1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
                1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,
                1,1,0,0,1,1,1,0,1,1,0,0,1,0,0,0};
*/
ShopAnalysis::ShopAnalysis(){

}

ShopAnalysis::~ShopAnalysis(){

}

void ShopAnalysis::buildingSortArea(Building* building){
    QVector<Floor*> allfloor=building->getFloors();
    for(int i=0; i<allfloor.size(); i++){
        sideAreaSort(allfloor[i]);
    }
}

void ShopAnalysis::sideAreaSort(Floor* floor){
    QList<FuncArea*> allAreas=floor->getFuncAreas();
    QPolygon floorOutline=floor->outline();
    int floorOutlineNum=floor->PointNum();
    FuncArea::SORT_TYPE areatype = FuncArea::MIDDLE_AREA;
    QList<FuncArea*> currentArea;
    QList<FuncArea*> leftArea(allAreas);
    QRect floorRect=floorOutline.boundingRect();
    int floorRectheight=floorRect.height();
    int floorRectwidth=floorRect.width();
    double floorDiagonal=sqrt(pow(floorRectheight,2)+pow(floorRectwidth,2));
    floorthreshold=floorDiagonal*ratio;
    areathreshold=floorDiagonal*(ratio-0.041);           //0.009
    QList<FuncArea*>::iterator current_iallAreas,left_iallAreas;

    //reset all areas to middle area
    QList<FuncArea*>::iterator iter;
    for(iter = allAreas.begin(); iter != allAreas.end(); iter++){
        (*iter)->setSortType(FuncArea::MIDDLE_AREA);
    }

    //area to floor
    threshold=floorthreshold;
    for(left_iallAreas = leftArea.begin(); left_iallAreas != leftArea.end(); ){
        QPolygon left_outline=(*left_iallAreas)->outline();

        bool linkmark=polygonTopolygon(floorOutline, left_outline);
        if(linkmark){
            areatype=FuncArea::SIDE_AREA;
            (*left_iallAreas)->setSortType(areatype);
            currentArea.append(*left_iallAreas);         //SIDEAREA_TYPEA range area add to currentArea
            left_iallAreas=leftArea.erase(left_iallAreas);//SIDEAREA_TYPEA range area move out from currentArea
        }else{
            ++left_iallAreas;
        }
    }


    int tempareatype=int(areatype);
    tempareatype++;
    areatype=(FuncArea::SORT_TYPE)tempareatype;
    //if currentArea is not empty, then sort deeply

    QList<FuncArea*> next_Area(currentArea);

    //area to area
    threshold=areathreshold;
     while((next_Area.size()!=0)&&(leftArea.size()!=0)){                            //next range is NULL, end sort
         next_Area.clear();                              //clear all next range area
         for(left_iallAreas=leftArea.begin();left_iallAreas!=leftArea.end(); ){
             QPolygon left_outline=(*left_iallAreas)->outline();
             for(current_iallAreas=currentArea.begin();current_iallAreas!=currentArea.end();current_iallAreas++){

                 QPolygon current_outline=(*current_iallAreas)->outline();

                 bool linkmark=polygonTopolygon(current_outline,left_outline);
                 if(linkmark){
                     (*left_iallAreas)->setSortType(areatype);
                     next_Area.append(*left_iallAreas);        //next range area add to  next_Area
                     left_iallAreas=leftArea.erase(left_iallAreas);  //next range area move out from
                     left_iallAreas--;
                     break;
                 }
             }

             ++left_iallAreas;

         }
         int tempareatype=int(areatype);
         tempareatype++;
         areatype=(FuncArea::SORT_TYPE)tempareatype;               //range increase
         currentArea.clear();                           //clear all current area
         currentArea.append(next_Area);                 //copy next range area to current range
     }
     //distinguish 中空区域 and 封闭区域
     QList<FuncArea*>::iterator iallAreas;
     for(iallAreas = allAreas.begin(); iallAreas != allAreas.end();iallAreas++){
         QString strtype=(*iallAreas)->type();
         if( (!strtype.compare("100", Qt::CaseInsensitive)) ||
             (!strtype.compare("000100", Qt::CaseInsensitive)) ||
             (!strtype.compare("300", Qt::CaseInsensitive)) ||
             (!strtype.compare("000300", Qt::CaseInsensitive)) ){
             (*iallAreas)->setSortType(FuncArea::UNSORTED);

         }else{
             if((*iallAreas)->sortType()==FuncArea::MIDDLE_AREA){
                 (*iallAreas)->setSortType(FuncArea::MIDDLE_AREA);  //set as middle store

             }else{
                 (*iallAreas)->setSortType(FuncArea::SIDE_AREA);  //set as side store

             }
         }
     }

}

double ShopAnalysis::pointToline(const QPoint &p0, const QPoint &p1, const QPoint &p2){
    double A=p1.y()-p2.y();
    double B=p2.x()-p1.x();
    double C=p1.x()*p2.y()-p2.x()*p1.y();
    double distance;
    if(A==0&&B==0){
        distance=sqrt(pow(p0.x()-p1.x(),2)+pow(p0.y()-p1.y(),2));
    }else{
        double a=pow(p0.x()-p1.x(),2)+pow(p0.y()-p1.y(),2);
        double b=pow(p0.x()-p2.x(),2)+pow(p0.y()-p2.y(),2);
        double c=pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2);
        if((a+b)==c){
            distance=0;
        }else if((b*b+c*c>=a*a)&&(a*a+c*c>=b*b)){
            distance=abs(A*p0.x()+B*p0.y()+C)/sqrt(A*A+B*B);
        }else{
            distance=sqrt(a)<sqrt(b)?sqrt(a):sqrt(b);
        }

    }
    return distance;
}


bool ShopAnalysis::polygonTopolygon(QPolygon &polygon1,QPolygon &polygon2){
    int polygon1num = polygon1.size(), polygon2num = polygon2.size();
    double mindistance0=pointToline(polygon2[0],polygon1[0],polygon1[1]);
    double distance0;
    for(int i=0;i<polygon2num;i++){
        for(int j=0;j<polygon1num;j++){
            if(j==(polygon1num-1)){
                distance0=pointToline(polygon2[i],polygon1[j],polygon1[0]);       //tail to head
            }else{
                distance0=pointToline(polygon2[i],polygon1[j],polygon1[j+1]);
            }
            if(distance0<mindistance0){
                mindistance0=distance0;
            }
        }
    }
    for(int i=0;i<polygon1num;i++){
        for(int j=0;j<polygon2num;j++){
            if(j==(polygon2num-1)){
                distance0=pointToline(polygon1[i],polygon2[j],polygon2[0]);       //tail to head
            }else{
                distance0=pointToline(polygon1[i],polygon2[j],polygon2[j+1]);
            }
            if(distance0<mindistance0){
                mindistance0=distance0;
            }
        }
    }
    if(mindistance0<threshold){
        return true;
    }else{
        return false;
    }
}
