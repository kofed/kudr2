#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lSaveROIButton, SIGNAL (released()), this, SLOT (onLSaveROIButton()));
    connect(ui->rSaveROIButton, SIGNAL (released()), this, SLOT (onRSaveROIButton()));
    connect(ui->lLoadDebugButton, SIGNAL (released()), this, SLOT (onLLoadDebugButton()));
    connect(ui->rLoadDebugButton, SIGNAL (released()), this, SLOT (onRLoadDebugButton()));
    connect(ui->saveDoorNumButton, SIGNAL (released()), this, SLOT (onSaveDoorNumButton()));

    ui->doorNumEdit->setValidator( new QIntValidator(1, 17, this) );

    controller.init();

    ui->lIpValueLabel->setText(controller.lIp);
    ui->rIpValueLabel->setText(controller.rIp);


    QPixmap pixmap(lImage);
    ui->lPngLabel.setPixmap(lImage);
    ui->lPngLabel.resize(pixmap.size());
    ui->lPngLabel.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLSaveROIButton(){

}

void MainWindow::onRSaveROIButton(){

}

void MainWindow::onLLoadDebugButton(){

}

void MainWindow::onRLoadDebugButton(){

}

void MainWindow::onSaveDoorNumButton(){
    //controller.saveDoorNum(ui->doorNumEdit->text().toInt());
}
