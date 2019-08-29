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

private slots:
    void onEditFocusLost(const QString &);

public:
    SizeEditWidget();

    SizeEditWidget(QString label);

    Size getSize();

    QString text();

    void clear();



signals:
    void redactionFinished();
};

#endif // SIZEEDITWIDGET_H
