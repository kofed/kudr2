#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"

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
private slots:
    void onLSaveROIButton();

    void onRSaveROIButton();

    void onLLoadDebugButton();

    void onRLoadDebugButton();

    void onSaveDoorNumButton();
};

#endif // MAINWINDOW_H
