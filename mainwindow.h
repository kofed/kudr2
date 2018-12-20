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

    QTableWidget * lCalibTable = new QTableWidget();
    QTableWidget * rCalibTable = new QTableWidget();

    QHBoxLayout * hLayout0 = new QHBoxLayout();
    QHBoxLayout * hSearchLayout = new QHBoxLayout();
    QVBoxLayout * vLayout0 = new QVBoxLayout();
    QComboBox * ipsCombo = new QComboBox();
    QPushButton * searchButton = new QPushButton();
    QLabel * searchDoorNumLabel = new QLabel();
    QLineEdit* searchDoorNum = new QLineEdit();

    PngWidget * lPngWidget;

    PngWidget * rPngWidget;

    CalibWidget * calibWidget;

    Logger * logger;


    void initIpsCombo();

    void   update();
private slots:
    void lUpdateImage();

    void rUpdateImage();


    void onLSaveROIButton();

    void onRSaveROIButton();

    void onSaveDoorNumButton();

    void onSearchButton();

    void onIpsComboSelected(int);

    void onRResolutionEdit(const QString &);

    void onLResolutionEdit(const QString &);

};

#endif // MAINWINDOW_H
