#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#include <QPushButton>
#include <QWidget>
#include "controller.h"

class SettingsWidget: public QWidget
{
private:
    Controller & controller;
private slots:
    void onSaveROIButton();

    void onResolutionEdit();


public:
    SettingsWidget(Controller & _controller);
};

#endif // SETTINGSWIDGET_H
