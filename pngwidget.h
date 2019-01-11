#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>

class PngWidget : public QLabel
{
    Q_OBJECT
public:
    PngWidget(QWidget *parent = 0);

    QRect selectionRect;

    QPoint* getCenter();

    virtual    ~PngWidget(){}
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:

    bool selectionStarted;
    void drawCircle(QPoint & center);
    QPoint  * center = NULL;

//private slots:
//    void saveSlot();

};

#endif // PNGWIDGET_H
