#include "scaletool.h"
#include "../gui/documentview.h"
#include "../core/scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QInputDialog>

#pragma execution_character_set("utf-8")

ScaleTool::ScaleTool(DocumentView *parent) :
    AbstractTool(parent), m_start(true), m_isCreating(false)
{
}

ScaleTool::~ScaleTool(){
    if(m_scaleLine){
        delete m_scaleLine;
        m_scaleLine = NULL;
    }
}

void ScaleTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(m_start){//first click
            m_scaleLine = new QGraphicsLineItem();
            m_scaleLine->setPen(QPen(QColor(56,56,56), 1));
            m_firstPoint = event->scenePos();
            m_scaleLine->setLine(QLineF(m_firstPoint, m_firstPoint));
            m_doc->scene()->addItem(m_scaleLine);

            m_start = false;
            m_isCreating = true;
        }else{//second click
            QPointF secondPoint = event->scenePos();
            QLineF line(m_firstPoint, secondPoint);
            m_scaleLine->setLine(line);

            bool ok;
            double inputLength = QInputDialog::getDouble(m_doc, "设定比例尺", "比例尺（单位：米）", 0,
                                                    -2147483647, 2147483647, 1, &ok);
            if (ok && inputLength != 0.0){
                double scale =  inputLength / line.length() * 10; // change to decimeter
                m_doc->scene()->addScale(scale);
            }
            delete m_scaleLine;
            m_scaleLine = NULL;
            m_start = true;
            m_isCreating = false;
        }
    }else if(event->button() == Qt::RightButton) {
        if(m_isCreating){
            delete m_scaleLine;
            m_scaleLine = NULL;
            m_start = true;
            m_isCreating = false;
        }
    }
}

void ScaleTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(m_isCreating) {
        QPointF secondPoint = event->scenePos();
        m_scaleLine->setLine(QLineF(m_firstPoint, secondPoint));
        m_scaleLine->update();
    }
}
