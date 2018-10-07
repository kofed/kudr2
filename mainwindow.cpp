#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include "pngwidget.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(logger)
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
    connect(ui->rResolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onRResolutionEdit(const QString &)));
    connect(ui->lResolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onLResolutionEdit(const QString &)));

    ui->doorNumEdit->setValidator( new QIntValidator(1, 17, this) );
    ui->searchDoorNum->setValidator(new QIntValidator(1, 17, this));

    ui->groupBox_3->setLayout(ui->verticalLayout_2);
    ui->groupBox_2->setLayout(ui->verticalLayout_3);

    ui->lResolutionEdit->setInputMask("9999\:9999");
    ui->lResolutionEdit->setText("400:300");
    onLResolutionEdit("640:480");

    ui->rResolutionEdit->setInputMask("9999\:9999");
    ui->rResolutionEdit->setText("400:300");
    onRResolutionEdit("640:480");

    logger = new Logger(ui->logListWidget);

    ui->lPositionCombo->addItem("Левая", LEFT);
    ui->lPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->addItem("Левая", LEFT);
    ui->rPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->setCurrentIndex(RIGHT);

    update();

    logger->log("Добро пожаловать!");

}

void MainWindow::update(){
    bool isLNull = controller.cameras[LEFT] == NULL;
    bool isRNull = controller.cameras[RIGHT] == NULL;

    ui->lSaveROIButton->setEnabled(!isLNull);
    ui->lShotButton->setEnabled(!isLNull);
    ui->lPositionCombo->setEnabled(!isLNull);
    ui->lLoadDebugButton->setEnabled(!isLNull);

    ui->rSaveROIButton->setEnabled(controller.cameras[RIGHT] != NULL);
    ui->rShotButton->setEnabled(controller.cameras[RIGHT] != NULL);
    ui->rPositionCombo->setEnabled(!isRNull);
    ui->rPositionCombo->setEnabled(!isRNull);
    ui->rLoadDebugButton->setEnabled(!isRNull);
    ui->lPngLabel->clear();

    ui->saveDoorNumButton->setEnabled(!isLNull || !isRNull);
    if(controller.cameras[LEFT] != NULL){
        ui->lIpValueLabel->setText(controller.cameras[LEFT]->toString());
        ui->doorNumEdit->setText(QString::number(controller.cameras[LEFT]->getDoorNumber()));        
        ui->lPositionCombo->setCurrentIndex(controller.cameras[LEFT]->getPosition());
    }else{
        ui->lIpValueLabel->setText("");

    }
    if(controller.cameras[RIGHT] != NULL){
        ui->rIpValueLabel->setText(controller.cameras[RIGHT]->toString());
        ui->doorNumEdit->setText(QString::number(controller.cameras[RIGHT]->getDoorNumber()));
        ui->rPositionCombo->setCurrentIndex(controller.cameras[RIGHT]->getPosition());

    }else{
        ui->rIpValueLabel->setText("");

    }

    ui->ipsCombo->clear();
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
    delete logger;
    delete ui;
}

void MainWindow::onLSaveROIButton(){
    logger->log("Сохраняю область на левое устройство");
    try{
        controller.saveROI(LEFT,
                           controller.lWidth,
                           controller.lHeight,
                           lPngWidget->selectionRect.left(),
                           lPngWidget->selectionRect.top(),
                           lPngWidget->selectionRect.right(),
                           lPngWidget->selectionRect.bottom());
       logger->log("Область сохранена");
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
        logger->log(QString("Область не сохранена. Ошибка: ") + e.what());
    }
}

void MainWindow::onRSaveROIButton(){
    logger->log("Сохраняю область на правое устройство");
    try{
        controller.saveROI(RIGHT,
                           controller.rWidth,
                           controller.rHeight,
                           rPngWidget->selectionRect.left(),
                           rPngWidget->selectionRect.top(),
                           rPngWidget->selectionRect.right(),
                           rPngWidget->selectionRect.bottom());
        logger->log("Область сохранена");
    }
    catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
        logger->log(QString("Область не сохранена. Ошибка: ") + e.what());
    }
}

void MainWindow::onLLoadDebugButton(){

}

void MainWindow::onRLoadDebugButton(){

}


void MainWindow::onLShotButton(){
    logger->log("Загружаю снимок с левого устройства");
    try{        
        controller.loadShot(LEFT, controller.rWidth, controller.rHeight);
        lUpdateImage();
        logger->log("Загрузка снимка успешно завершена");
    }
    catch(const std::exception & e){
        logger->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onRShotButton(){
    logger->log("Загружаю снимок с правого устройства");
    try{
        controller.loadShot(RIGHT, controller.rWidth, controller.rHeight);
        rUpdateImage();
        logger->log("Загрузка снимка успешно завершена");
    }
    catch(const std::exception & e){
        logger->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}

void MainWindow::onSaveDoorNumButton(){
    logger->log("Сохраняю номер двери и позицию");
        int doorNumber = ui->doorNumEdit->text().toInt();

        Position lPos = ui->lPositionCombo->currentData().value<Position>();
        CameraIp newLeftCamera(doorNumber, lPos);

        Position rPos = ui->rPositionCombo->currentData().value<Position>();
        CameraIp newRightCamera(doorNumber, rPos);

        logger->log("... на левую камеру");
        try{
            controller.saveCamera(LEFT, newLeftCamera);
            logger->log("... успешно завершено");
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
            logger->log(QString("... произошла ошибка ") + e.what());
        }
        logger->log("на правую камеру");
        try{
            controller.saveCamera(RIGHT, newRightCamera);
            logger->log("... успешно завершено");
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
            logger->log(QString("... произошла ошибка ") + e.what());
        }
        update();
}

void MainWindow::onSearchButton(){
    try{
        QString searchDoorNum = ui->searchDoorNum->text();
        if(!searchDoorNum.isEmpty()){
            logger->log("Ищу устройства с номером двери " + searchDoorNum);
           controller.setCameras(CameraIp(searchDoorNum.toInt(), LEFT));
           update();
           logger->log("... поиск завершен");
        } else{
            logger->log("Выполняю сканирование по диапазону адресов");
            controller.pingController.scan();
            initIpsCombo();            
            logger->log("Сканирование по диапазону адресов успешно завершено");
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


void MainWindow::onRResolutionEdit(const QString &){
    QString sResolution = ui->rResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.rWidth = QString(slResolution.at(0)).toInt();
    controller.rHeight = QString(slResolution.at(1)).toInt();

}

void MainWindow::onLResolutionEdit(const QString &){
    QString sResolution = ui->lResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.lWidth = QString(slResolution.at(0)).toInt();
    controller.lHeight = QString(slResolution.at(1)).toInt();

}
