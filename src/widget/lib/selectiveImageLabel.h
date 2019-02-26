#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>


using namespace std;

class SelectiveImageLabel : public QLabel
{
    Q_OBJECT
public:
    SelectiveImageLabel(QWidget *parent = 0);

    QRect getSelectionRect();

    QPoint getPoint();

    virtual    ~SelectiveImageLabel(){}
signals:
    void rectSelected(const QRect & rect);
    void pointSelected(const QPoint  & center);


protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QRect selectionRect = QRect(0, 0, 0, 0);
    QPoint point = QPoint(0,0);
    bool selectionStarted;
    void drawCircle();
};
#endif // PNGWIDGET_H
