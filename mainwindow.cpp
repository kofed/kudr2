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
    connect(ui->searchButton, SIGNAL(released()), this, SLOT(onSearchButton()));
    connect(ui->ipsCombo, SIGNAL(activated(int)), this, SLOT(onIpsComboSelected(int )));
    connect(ui->rResolutionEdit, SIGNAL(editingFinished()), this, SLOT(onRResolutionEdit()));
    connect(ui->lResolutionEdit, SIGNAL(editingFinished()), this, SLOT(onLResolutionEdit()));

    ui->doorNumEdit->setValidator( new QIntValidator(1, 17, this) );
    ui->searchDoorNum->setValidator(new QIntValidator(1, 17, this));

    ui->groupBox_3->setLayout(ui->verticalLayout_2);
    ui->groupBox_2->setLayout(ui->verticalLayout_3);

    ui->lResolutionEdit->setInputMask("9999\:9999");
    ui->lResolutionEdit->setText("640:480");
    onLResolutionEdit();

    ui->rResolutionEdit->setInputMask("9999\:9999");
    ui->rResolutionEdit->setText("640:480");
    onRResolutionEdit();

    ui->lPositionCombo->addItem("Левая", LEFT);
    ui->lPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->addItem("Левая", LEFT);
    ui->rPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->setCurrentIndex(RIGHT);
    //initIpsCombo();
    update();

}

void MainWindow::update(){
    if(controller.cameras[LEFT] != NULL){
        ui->lIpValueLabel->setText(controller.cameras[LEFT]->toString());
        ui->doorNumEdit->setText(QString::number(controller.cameras[LEFT]->getDoorNumber()));
     }
    if(controller.cameras[RIGHT] != NULL){
        ui->rIpValueLabel->setText(controller.cameras[RIGHT]->toString());
        ui->doorNumEdit->setText(QString::number(controller.cameras[RIGHT]->getDoorNumber()));
    }

    if(controller.cameras[LEFT] != NULL)
        ui->lPositionCombo->setCurrentIndex(controller.cameras[LEFT]->getPosition());
    if(controller.cameras[RIGHT] != NULL)
        ui->rPositionCombo->setCurrentIndex(controller.cameras[RIGHT]->getPosition());
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
    if(controller.cameras[LEFT] == NULL)
        return;
    try{
        QString imageFile = controller.imagePattern.arg(controller.cameras[LEFT]->getPosition());
        QPixmap pixmap(imageFile);
        lPngWidget = new PngWidget(ui->lPngLabel);
        ui->lPngLabel->setPixmap(pixmap);
        lPngWidget->resize(pixmap.size());
        lPngWidget->show();
        qApp->processEvents();
    }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
}

void MainWindow::rUpdateImage(){
    try{
        QPixmap pixmap(controller.imagePattern.arg(RIGHT));
        rPngWidget = new PngWidget(ui->rPngLabel);
        ui->rPngLabel->setPixmap(pixmap);
        rPngWidget->resize(pixmap.size());
        rPngWidget->show();
        qApp->processEvents();
    }catch(const std::exception & e){
         QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLSaveROIButton(){
    try{
        controller.saveROI(LEFT,
                           controller.lWidth,
                           controller.lHeight,
                           lPngWidget->selectionRect.left(),
                           lPngWidget->selectionRect.top(),
                           lPngWidget->selectionRect.right(),
                           lPngWidget->selectionRect.bottom());
        QMessageBox::warning(this, "Выполнено!", "Координаты сохранены");
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onRSaveROIButton(){
    try{
        controller.saveROI(RIGHT,
                           controller.rWidth,
                           controller.rHeight,
                           rPngWidget->selectionRect.left(),
                           rPngWidget->selectionRect.top(),
                           rPngWidget->selectionRect.right(),
                           rPngWidget->selectionRect.bottom());
        QMessageBox::warning(this, "Выполнено!", "Координаты сохранены");
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
        controller.loadShot(LEFT, controller.rWidth, controller.rHeight);
        lUpdateImage();
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onRShotButton(){
    try{
        controller.loadShot(RIGHT, controller.rWidth, controller.rHeight);
        rUpdateImage();
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onSaveDoorNumButton(){

        int doorNumber = ui->doorNumEdit->text().toInt();

        Position lPos = ui->lPositionCombo->currentData().value<Position>();
        CameraIp newLeftCamera(doorNumber, lPos);

        Position rPos = ui->rPositionCombo->currentData().value<Position>();
        CameraIp newRightCamera(doorNumber, rPos);
        try{
            controller.saveCamera(LEFT, newLeftCamera);
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
        }
        try{
            controller.saveCamera(RIGHT, newRightCamera);
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
        }
        update();
}

void MainWindow::onSearchButton(){
    try{
        if(!ui->searchDoorNum->text().isEmpty()){
           controller.setCameras(CameraIp(ui->searchDoorNum->text().toInt(), LEFT));
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


void MainWindow::onRResolutionEdit(){
    QString sResolution = ui->rResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.rWidth = QString(slResolution.at(0)).toInt();
    controller.rHeight = QString(slResolution.at(1)).toInt();

}

void MainWindow::onLResolutionEdit(){
    QString sResolution = ui->lResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.lWidth = QString(slResolution.at(0)).toInt();
    controller.lHeight = QString(slResolution.at(1)).toInt();

}
