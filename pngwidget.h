#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>
#include "calibcontroller.h"

class PngWidget : public QLabel
{
    Q_OBJECT
public:
    PngWidget(QWidget *parent = 0);

    QRect getSelectionRect();

    Point2i getCenter();

    virtual    ~PngWidget(){}
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QRect selectionRect;
    Point2i center;
    bool selectionStarted;
    void drawCircle();
};

#endif // PNGWIDGET_H
