#include "pngwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

PngWidget::PngWidget(Point2i & _center, QWidget *parent)
: center(_center), QLabel(parent)
{
    selectionStarted=false;
}

void PngWidget::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,255,255,120)));

    painter.drawRect(selectionRect);

    drawCircle();
}

void PngWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::RightButton){        
        center.x = e->x();
        center.y = e->y();
        repaint();
    }
    else{
        selectionStarted=true;
        selectionRect.setTopLeft(e->pos());
        selectionRect.setBottomRight(e->pos());
    }
}

void PngWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted){
        selectionRect.setBottomRight(e->pos());
        repaint();
    }
}

void PngWidget::mouseReleaseEvent(QMouseEvent *e)
{
    selectionStarted=false;
}

void PngWidget::drawCircle(){
    const int diameter = 15;
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(255,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,0,255,120)));
    painter.drawEllipse(QRect(center.x - diameter/2, center.y -diameter / 2, diameter, diameter));
}
