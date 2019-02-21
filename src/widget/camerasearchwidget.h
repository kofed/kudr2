#ifndef CAMERASEARCHWIDGET_H
#define CAMERASEARCHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "pingcontroller.h"
#include "controller.h"

class CameraSearchWidget : public QWidget{
    Q_OBJECT

public:
    explicit CameraSearchWidget(Controller & _controller, QWidget *parent = 0);

    virtual ~CameraSearchWidget(){}
private:
    Controller & controller;

    PingController pingController;

    QPushButton * searchButton = new QPushButton("Искать");

    QLineEdit* doorNumEdit = new QLineEdit();

    QComboBox* ipsCombo = new QComboBox();

    void initIpsCombo();



private slots:
    void onSearchButton();

    void onIpsComboSelected(int idx);

signals:
    void ipSelected();
};

#endif // CAMERASEARCHWIDGET_H
