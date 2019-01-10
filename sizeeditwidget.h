#ifndef SIZEEDITWIDGET_H
#define SIZEEDITWIDGET_H

#include <QLineEdit>

class SizeEditWidget : public QLineEdit
{
    Q_OBJECT
public:
    SizeEditWidget();

    Size getSize();
};

#endif // SIZEEDITWIDGET_H
