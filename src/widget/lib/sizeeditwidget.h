#ifndef SIZEEDITWIDGET_H
#define SIZEEDITWIDGET_H

#include <QLineEdit>
#include "opencv2/opencv.hpp"

using namespace cv;

class SizeEditWidget : public QWidget
{
    Q_OBJECT

private:
    QLineEdit * edit;
public:
    SizeEditWidget();

    SizeEditWidget(QString label);

    Size getSize();

    QString text();

    void clear();
};

#endif // SIZEEDITWIDGET_H
