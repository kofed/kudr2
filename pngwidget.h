#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>
#include "calibcontroller.h"

class PngWidget : public QLabel
{
    Q_OBJECT
public:
    PngWidget(Point2i & _center, QWidget *parent = 0);

    QRect selectionRect;

    Point2i & center;

    virtual    ~PngWidget(){}
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:

    bool selectionStarted;
    void drawCircle();
//private slots:
//    void saveSlot();

};

#endif // PNGWIDGET_H
