#include "selectiveImageLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

SelectiveImageLabel::SelectiveImageLabel(QWidget *parent): QLabel(parent)
{
    selectionStarted=false;
}

void SelectiveImageLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,255,255,120)));

    painter.drawRect(selectionRect);

    drawCircle();
}

void SelectiveImageLabel::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::RightButton){        
        center.x = e->x();
        center.y = e->y();

        repaint();
        emit centerSelected(center);
    }
    else{
        selectionStarted=true;
        selectionRect.setTopLeft(e->pos());
        selectionRect.setBottomRight(e->pos());
    }
}

void SelectiveImageLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted){
        selectionRect.setBottomRight(e->pos());
        repaint();
    }
}

void SelectiveImageLabel::mouseReleaseEvent(QMouseEvent *e)
{
    selectionStarted=false;
    if(e->button()==Qt::LeftButton){

        emit rectSelected(selectionRect);
    }
}

void SelectiveImageLabel::drawCircle(){
    const int diameter = 15;
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(255,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,0,255,120)));
    painter.drawEllipse(QRect(center.x - diameter/2, center.y -diameter / 2, diameter, diameter));

}

QRect SelectiveImageLabel::getSelectionRect(){
    if(selectionRect.width() <= 0 || selectionRect.height() <= 0){
        throw new runtime_error("Выделите прямоугольник на рисунке");
    }

    return selectionRect;
}

cv::Point2i SelectiveImageLabel::getCenter(){
    if(center.x <= 0 || center.y <= 0){
        throw new runtime_error("Выделите центр на рисунке");
    }

    return center;
}
