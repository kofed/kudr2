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
public slots:
    void setScaleFactor(double _scaleFactor);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QRect scaleRect(double scaleFactor);
    QPoint scalePoint(double scaleFactor);
    QRect selectionRect = QRect(0, 0, 0, 0);
    QPoint point = QPoint(0,0);
    bool selectionStarted;
    void drawCircle();
    double scaleFactor;
};
#endif // PNGWIDGET_H
