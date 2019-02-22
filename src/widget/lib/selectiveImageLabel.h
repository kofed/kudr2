#ifndef PNGWIDGET_H
#define PNGWIDGET_H

#include <QLabel>
#include <QMenu>
#include "opencv2/opencv.hpp"

using namespace std;

class SelectiveImageLabel : public QLabel
{
    Q_OBJECT
public:
    SelectiveImageLabel(QWidget *parent = 0);

    QRect getSelectionRect();

    cv::Point2i getCenter();

    virtual    ~SelectiveImageLabel(){}
signals:
    void rectSelected(const QRect & rect);
    void centerSelected(const cv::Point2i  & center);


protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QRect selectionRect = QRect(0, 0, 0, 0);
    cv::Point2i center = cv::Point2i(0,0);
    bool selectionStarted;
    void drawCircle();
};

Q_DECLARE_METATYPE(cv::Point2i)

#endif // PNGWIDGET_H
