#include "selectiveImageLabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QFileDialog>

SelectiveImageLabel::SelectiveImageLabel(QWidget *parent): QLabel(parent)
{
    selectionStarted=false;
    scaleFactor = 1;
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
        point = e->pos();

        repaint();
        emit pointSelected(getPoint());
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
        if(selectionRect.width() <= 0 || selectionRect.height() <= 0){
            //throw runtime_error("Выделите прямоугольник на рисунке");
            return;
        }

        scaleRect(1/scaleFactor);


        emit rectSelected(selectionRect);
    }
}

void SelectiveImageLabel::drawCircle(){
    const int diameter = 15;
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(255,0,0,180)),1,Qt::DashLine));
    painter.setBrush(QBrush(QColor(255,0,255,120)));
    painter.drawEllipse(QRect(point.x() - diameter/2, point.y() -diameter / 2, diameter, diameter));
}

QPoint SelectiveImageLabel::getPoint(){
    if(point.x() <= 0 || point.y() <= 0){
        throw runtime_error("Выделите точку на рисунке");
    }

    return scalePoint(1/scaleFactor);
}

QRect SelectiveImageLabel::scaleRect(const double scaleFactor){
    return QRect(selectionRect.x() * scaleFactor,
                     selectionRect.y() * scaleFactor,
                     selectionRect.width() * scaleFactor,
                     selectionRect.height() * scaleFactor);
}

QPoint SelectiveImageLabel::scalePoint(const double scaleFactor){
    return QPoint(point.x() * scaleFactor, point.y() * scaleFactor);
}

void SelectiveImageLabel::setScaleFactor(double _scaleFactor){
    point = scalePoint(_scaleFactor / scaleFactor);
    selectionRect = scaleRect(_scaleFactor / scaleFactor);
    scaleFactor = _scaleFactor;
}
