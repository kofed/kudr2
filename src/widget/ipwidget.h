#ifndef IPWIDGET_H
#define IPWIDGET_H

#include <QComboBox>
#include <QWidget>
#include "controller.h"
#include <QLineEdit>

class IpWidget : public QWidget
{
    Q_OBJECT

private:
    Controller & controller;
    QLineEdit* doorNumEdit = new QLineEdit();
    QComboBox * lCombo = new QComboBox;
    QComboBox * rCombo = new QComboBox;

private slots:
    void onSaveDoorNumButton();

public:
    IpWidget(Controller & _controller);
};

#endif // IPWIDGET_H
