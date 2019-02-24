#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#include <QPushButton>
#include <QWidget>
#include "controller/controller.h"
#include "calibcontroller.h"

class SettingsWidget: public QWidget
{
    Q_OBJECT

public:
    SettingsWidget(Controller & _controller, CalibController & _calibController);

    virtual ~SettingsWidget(){}

public slots:
    void setEnabled();

private:
    Controller & controller;

    CalibController & calibController;

    QPushButton * shotButton;

private slots:
    void onSaveROIButton();

    void onResolutionEdit();

    void onShotButton();

    void openFile();
signals:
    void updateChessBoardImage();

};

#endif // SETTINGSWIDGET_H
