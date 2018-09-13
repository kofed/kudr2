#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include "pngwidget.h"

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
    connect(ui->lShotButton, SIGNAL (released()), this, SLOT (onLShotButton()));
    connect(ui->rShotButton, SIGNAL (released()), this, SLOT (onRShotButton()));
    connect(ui->saveDoorNumButton, SIGNAL (released()), this, SLOT (onSaveDoorNumButton()));


    ui->doorNumEdit->setValidator( new QIntValidator(1, 17, this) );

    ui->groupBox_3->setLayout(ui->verticalLayout_2);
    ui->groupBox_2->setLayout(ui->verticalLayout_3);


    controller.init();

    ui->lIpValueLabel->setText(controller.lCamera.toString());
    ui->rIpValueLabel->setText(controller.rCamera.toString());

    ui->doorNumEdit->setText(QString::number(controller.lCamera.getDoorNumber()));

    lUpdateImage();
    rUpdateImage();
    qApp->processEvents();
}

void MainWindow::lUpdateImage(){
    QPixmap pixmap(controller.imagePattern.arg(controller.lCamera.getPosition()));
    lPngWidget = new PngWidget(ui->lPngLabel);
    ui->lPngLabel->setPixmap(pixmap);
    lPngWidget->resize(pixmap.size());
    lPngWidget->show();
}

void MainWindow::rUpdateImage(){
    QPixmap pixmap(controller.imagePattern.arg(controller.rCamera.getPosition()));
    rPngWidget = new PngWidget(ui->rPngLabel);
    ui->rPngLabel->setPixmap(pixmap);
    rPngWidget->resize(pixmap.size());
    rPngWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLSaveROIButton(){
    controller.saveROI(controller.lCamera,
                       lPngWidget->selectionRect.left(),
                       lPngWidget->selectionRect.top(),
                       lPngWidget->selectionRect.right(),
                       lPngWidget->selectionRect.bottom());
}

void MainWindow::onRSaveROIButton(){
    controller.saveROI(controller.lCamera,
                       lPngWidget->selectionRect.left(),
                       lPngWidget->selectionRect.top(),
                       lPngWidget->selectionRect.right(),
                       lPngWidget->selectionRect.bottom());
}

void MainWindow::onLLoadDebugButton(){

}

void MainWindow::onRLoadDebugButton(){

}


void MainWindow::onLShotButton(){
    controller.loadShot(controller.lCamera);
    lUpdateImage();
}

void MainWindow::onRShotButton(){
    controller.loadShot(controller.rCamera);
    rUpdateImage();
}

void MainWindow::onSaveDoorNumButton(){
    controller.saveDoorNum(ui->doorNumEdit->text().toInt());
}
