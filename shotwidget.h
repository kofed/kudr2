#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H

#include "imageviewer.h"
#include "sizeeditwidget.h"
#include <QRadioButton>
#include "controller.h"
#include "calibcontroller.h"

class ShotWidget
{
private:
    ImageViewer * viewer;

    SizeEditWidget * positionEdit = new SizeEditWidget;

    QRadioButton * deleteRadioButton = new QRadioButton("Удалить угол");
    QRadioButton * addRadioButton = new QRadioButton("Добавить угол");

    //Controller & controller;
    //CalibController & calibController;
public:
    ShotWidget();
};

#endif // SHOTWIDGET_H
