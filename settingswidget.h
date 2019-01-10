#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#include <QPushButton>
#include <QWidget>
#include "controller.h"

class SettingsWidget: public QWidget
{
    Q_OBJECT

public:
    SettingsWidget(Controller & _controller);

    virtual ~SettingsWidget(){}

public slots:
    void setEnabled();



private:
    Controller & controller;
private slots:
    void onSaveROIButton();

    void onResolutionEdit();


};

#endif // SETTINGSWIDGET_H
