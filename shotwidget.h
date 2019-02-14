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

    SizeEditWidget * positionEdit;

    QRadioButton * deleteRadioButton, addRadioButton;

    //Controller & controller;
    //CalibController & calibController;
public:
    ShotWidget();
};

#endif // SHOTWIDGET_H
