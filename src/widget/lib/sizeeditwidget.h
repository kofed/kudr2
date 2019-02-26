#ifndef SIZEEDITWIDGET_H
#define SIZEEDITWIDGET_H

#include <QLineEdit>
#include "opencv2/opencv.hpp"

using namespace cv;

class SizeEditWidget : public QLineEdit
{
    Q_OBJECT
public:
    SizeEditWidget();

    SizeEditWidget(QString label);

    Size getSize();
};

#endif // SIZEEDITWIDGET_H
