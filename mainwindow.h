#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "pngwidget.h"
#include "position.h"

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

    PngWidget * lPngWidget;

    PngWidget * rPngWidget;

    void lUpdateImage();

    void rUpdateImage();

    void initIpsCombo();

    void   update();
private slots:
    void onLSaveROIButton();

    void onRSaveROIButton();

    void onLLoadDebugButton();

    void onRLoadDebugButton();

    void onSaveDoorNumButton();

    void onLShotButton();

    void onRShotButton();

    void onSearchButton();

    void onIpsComboSelected(int);

    void onRResolutionEdit();

    void onLResolutionEdit();
};

#endif // MAINWINDOW_H
