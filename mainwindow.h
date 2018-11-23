#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "pngwidget.h"
#include "position.h"
#include "calibwidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

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

    QHBoxLayout * hLayout0 = new QHBoxLayout();
    QHBoxLayout * hSearchLayout = new QHBoxLayout();
    QHBoxLayout * hSaveDoorNumLayout = new QHBoxLayout();
    QVBoxLayout * vLayout0 = new QVBoxLayout();
    QComboBox * ipsCombo = new QComboBox();
    QPushButton * searchButton = new QPushButton();
    QPushButton * saveDoorNumButton = new QPushButton();
    QPushButton * saveShotButton = new QPushButton("сохр. снимок");
    QLabel * searchDoorNumLabel = new QLabel();
    QLineEdit* searchDoorNum = new QLineEdit();
    QLineEdit* doorNumEdit = new QLineEdit();

    PngWidget * lPngWidget;

    PngWidget * rPngWidget;

   CalibWidget * lCalibWidget, *rCalibWidget;

    Logger * logger;

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

    void onRResolutionEdit(const QString &);

    void onLResolutionEdit(const QString &);
};

#endif // MAINWINDOW_H
