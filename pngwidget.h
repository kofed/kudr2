#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>

class PngWidget
{
    Q_OBJECT

public:
    PngWidget(QWidget *parent = 0);
    ~PngWidget();
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:

    bool selectionStarted;
    QRect selectionRect;
    QMenu contextMenu;
//private slots:
//    void saveSlot();

};

#endif // PNGWIDGET_H
