#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller/controller.h"
#include "lib/selectiveImageLabel.h"
#include "position.h"
#include "calibwidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include "shotswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     Controller controller;

private:
    Ui::MainWindow *ui;

    QPushButton * saveDoorNumButton = new QPushButton();
    QPushButton * saveShotButton = new QPushButton("сохр. снимок");
    QLabel * searchDoorNumLabel = new QLabel();

    CalibWidget * calibWidget;

    Logger * logger;


    void initIpsCombo();

    void initSettingsTab();

    void initShotTab();

    void   update();
private slots:
    void onIpchanged(const QString & ip);

};

#endif // MAINWINDOW_H
