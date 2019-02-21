#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H
#include <QPushButton>
#include <QWidget>
#include "src/controller/controller.h"

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
