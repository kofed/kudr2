#ifndef IPWIDGET_H
#define IPWIDGET_H

#include <QComboBox>
#include <QWidget>

class IpWidget : public QWidget
{
    Q_OBJECT

private:
    Controller & controller;

private slots:
    void onSaveDoorNumButton();

public:
    IpWidget(Controller & _controller);
};

#endif // IPWIDGET_H
