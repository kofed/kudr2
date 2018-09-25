#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include "pngwidget.h"
#include <QMessageBox>

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
    connect(ui->searchButton, SIGNAL(released()), this, SLOT(onSearchButton()));
    connect(ui->ipsCombo, SIGNAL(activated(int)), this, SLOT(onIpsComboSelected(int )));

    ui->doorNumEdit->setValidator( new QIntValidator(1, 17, this) );
    ui->searchDoorNum->setValidator(new QIntValidator(1, 17, this));

    ui->groupBox_3->setLayout(ui->verticalLayout_2);
    ui->groupBox_2->setLayout(ui->verticalLayout_3);

    controller.init();


    //initIpsCombo();
    update();

}

void MainWindow::update(){
    if(controller.lCamera != NULL){
        ui->lIpValueLabel->setText(controller.lCamera->toString());
        ui->doorNumEdit->setText(QString::number(controller.lCamera->getDoorNumber()));
     }
    if(controller.rCamera != NULL){
        ui->rIpValueLabel->setText(controller.rCamera->toString());
        ui->doorNumEdit->setText(QString::number(controller.lCamera->getDoorNumber()));
    }
    //lUpdateImage();
    //rUpdateImage();
}

void MainWindow::initIpsCombo(){
    ui->ipsCombo->clear();
    for(auto i : controller.pingController.activeIps){
        ui->ipsCombo->addItem(i);
    }
}

void MainWindow::lUpdateImage(){
    if(controller.lCamera == NULL)
        return;
    //QString imageFile = "/tmp/left.png";
    QString imageFile = controller.imagePattern.arg(controller.lCamera->getPosition());
    QPixmap pixmap(imageFile);
    lPngWidget = new PngWidget(ui->lPngLabel);
    ui->lPngLabel->setPixmap(pixmap);
    lPngWidget->resize(pixmap.size());
    lPngWidget->show();
    qApp->processEvents();
}

void MainWindow::rUpdateImage(){
    if(controller.rCamera == NULL){
        return;
    }
    QPixmap pixmap(controller.imagePattern.arg(controller.rCamera->getPosition()));
    rPngWidget = new PngWidget(ui->rPngLabel);
    ui->rPngLabel->setPixmap(pixmap);
    rPngWidget->resize(pixmap.size());
    rPngWidget->show();
    qApp->processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLSaveROIButton(){
    try{
        controller.saveROI(controller.lCamera,
                           lPngWidget->selectionRect.left(),
                           lPngWidget->selectionRect.top(),
                           lPngWidget->selectionRect.right(),
                           lPngWidget->selectionRect.bottom());
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onRSaveROIButton(){
    try{
        controller.saveROI(controller.lCamera,
                       lPngWidget->selectionRect.left(),
                       lPngWidget->selectionRect.top(),
                       lPngWidget->selectionRect.right(),
                       lPngWidget->selectionRect.bottom());
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onLLoadDebugButton(){

}

void MainWindow::onRLoadDebugButton(){

}


void MainWindow::onLShotButton(){
    try{
        controller.loadShot(controller.lCamera);
        lUpdateImage();
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onRShotButton(){
    try{
        controller.loadShot(controller.rCamera);
        rUpdateImage();
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onSaveDoorNumButton(){
    try{
        controller.saveDoorNum(ui->doorNumEdit->text().toInt());
        update();
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onSearchButton(){
    try{
        if(!ui->searchDoorNum->text().isEmpty()){
           controller.searchOnDoorNum(ui->searchDoorNum->text().toInt());
           update();
        } else{
            controller.pingController.scan();
            initIpsCombo();
        }
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onIpsComboSelected(int item){
    try{
        controller.setCameras(CameraIp(ui->ipsCombo->itemText(item)));
        update();
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}
