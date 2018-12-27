#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include "pngwidget.h"
#include <QMessageBox>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) :
    logger(new Logger(this)),
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(logger)
{
    //ui->setupUi(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setStretch(0, 1);
    tabWidget = new QTabWidget(this);
    initSettingsTab();
    initShotTab();
    tabWidget->addTab(settingsTab, "Параметры");
    tabWidget->addTab(shotTab, "Снимки");
    mainLayout->addWidget(tabWidget);









  /*

    ui->verticalLayout->addLayout(hLayout0);


    vLayout0->addLayout(hSearchLayout);
    vLayout0->addWidget(ipsCombo);
    vLayout0->addLayout(hSaveDoorNumLayout);

    hSaveDoorNumLayout->addWidget(doorNumEdit);
    hSaveDoorNumLayout->addWidget(saveDoorNumButton);
    hLayout0->addWidget((QWidget*) logger->logListWidget);

    logger->logListWidget->setMaximumHeight(100);*/



  /*  connect(ui->lSaveROIButton, SIGNAL (released()), this, SLOT (onLSaveROIButton()));
    connect(ui->rSaveROIButton, SIGNAL (released()), this, SLOT (onRSaveROIButton()));
    connect(saveDoorNumButton, SIGNAL (released()), this, SLOT (onSaveDoorNumButton()));
    connect(searchButton, SIGNAL(released()), this, SLOT(onSearchButton()));
    connect(ipsCombo, SIGNAL(activated(int)), this, SLOT(onIpsComboSelected(int )));
    connect(ui->rResolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onRResolutionEdit(const QString &)));
    connect(ui->lResolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onLResolutionEdit(const QString &)));
    connect(calibWidget, SIGNAL(updateChessBoardImage()), this, SLOT(rUpdateImage()));


    doorNumEdit->setValidator( new QIntValidator(1, 17, this) );
    searchDoorNum->setValidator(new QIntValidator(1, 17, this));

    ui->groupBox_3->setLayout(ui->verticalLayout_2);
    ui->groupBox_2->setLayout(ui->verticalLayout_3);



    ui->lResolutionEdit->setInputMask("999\:999");
    ui->lResolutionEdit->setText("400:300");
    onLResolutionEdit("640:480");

    ui->rResolutionEdit->setInputMask("999\:999");
    ui->rResolutionEdit->setText("400:300");
    onRResolutionEdit("640:480");

    ui->lPositionCombo->addItem("Левая", LEFT);
    ui->lPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->addItem("Левая", LEFT);
    ui->rPositionCombo->addItem("Правая", RIGHT);

    ui->rPositionCombo->setCurrentIndex(RIGHT);

    //ui->horizontalLayout_2->addWidget(saveShotButton);

    saveDoorNumButton->setEnabled(false);

    update();

    logger->log("Добро пожаловать!");*/

}

void MainWindow::initShotTab(){
    shotTab = new QWidget(this);
    QHBoxLayout* shotLayout = new QHBoxLayout(shotTab);
    shotTab->setLayout(shotLayout);

    QLabel* lPngLabel = new QLabel(shotTab);
    shotLayout->addWidget(lPngLabel);

    QLabel* rPngLabel = new QLabel(shotTab);
    shotLayout->addWidget(rPngLabel);
}

void MainWindow::initSettingsTab(){
    settingsTab = new QWidget(this);

    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsTab);


    calibTable = new QTableWidget(settingsTab);
    calibWidget = new CalibWidget(settingsTab, calibTable, controller);

    settingsLayout->addWidget(calibTable, 1);
    settingsLayout->addWidget((QWidget*)calibWidget);

    searchButton->setText("Поиск");
        searchDoorNumLabel->setText("по номеру двери");
        saveDoorNumButton->setText("Сохр. номер двери");

        hLayout0->addLayout(vLayout0);
        hSearchLayout->addWidget(searchButton);
        hSearchLayout->addWidget(searchDoorNumLabel);
        hSearchLayout->addWidget(searchDoorNum);
         settingsLayout->addStretch(1);

        settingsLayout->addLayout(hLayout0);

        settingsTab->setLayout(settingsLayout);

}

void MainWindow::update(){
    bool isLNull = controller.cameras[LEFT] == NULL;
    bool isRNull = controller.cameras[RIGHT] == NULL;

    ui->lSaveROIButton->setEnabled(!isLNull);
    ui->lPositionCombo->setEnabled(!isLNull);

    ui->rSaveROIButton->setEnabled(controller.cameras[RIGHT] != NULL);
    ui->rPositionCombo->setEnabled(!isRNull);
    ui->rPositionCombo->setEnabled(!isRNull);
    ui->lPngLabel->clear();
  calibWidget->setEnabled(true);//MOCK  calibWidget->setEnabled(!isLNull || !isRNull);

    //ui->saveDoorNumButton->setEnabled(!isLNull || !isRNull);
    if(controller.cameras[LEFT] != NULL){
        ui->lIpValueLabel->setText(controller.cameras[LEFT]->toString());
        doorNumEdit->setText(QString::number(controller.cameras[LEFT]->getDoorNumber()));
        ui->lPositionCombo->setCurrentIndex(controller.cameras[LEFT]->getPosition());
    }else{
        ui->lIpValueLabel->setText("");

    }
    if(controller.cameras[RIGHT] != NULL){
        ui->rIpValueLabel->setText(controller.cameras[RIGHT]->toString());
        doorNumEdit->setText(QString::number(controller.cameras[RIGHT]->getDoorNumber()));
        ui->rPositionCombo->setCurrentIndex(controller.cameras[RIGHT]->getPosition());

    }else{
        ui->rIpValueLabel->setText("");

    }

    ipsCombo->clear();

}

void MainWindow::initIpsCombo(){
    ipsCombo->clear();
    for(auto i : controller.pingController.activeIps){
        ipsCombo->addItem(i);
    }
}

void MainWindow::lUpdateImage(){
    if(controller.cameras[LEFT] == NULL)
        return;
    try{
        QString imageFile = "/tmp/left.png"; /*controller.getImgFileName(LEFT);*/
        QPixmap pixmap(imageFile);
        lPngWidget = new PngWidget(ui->lPngLabel);
        ui->lPngLabel->setPixmap(pixmap);
        lPngWidget->resize(pixmap.size());
        lPngWidget->show();
        calibWidget->setEnabled(true);
        qApp->processEvents();
    }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
}

void MainWindow::rUpdateImage(){
    try{
        QString imageFile = "/tmp/right.png";//controller.imagePattern.arg(RIGHT)
        QPixmap pixmap(imageFile);
        rPngWidget = new PngWidget(ui->rPngLabel);
        ui->rPngLabel->setPixmap(pixmap);
        rPngWidget->resize(pixmap.size());
        rPngWidget->show();
        calibWidget->setEnabled(true);
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

void MainWindow::onSaveDoorNumButton(){
    logger->log("Сохраняю номер двери и позицию");
        int doorNumber = doorNumEdit->text().toInt();

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
        QString searchDoorNum /*= ui->searchDoorNum->text()*/;
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
        controller.setCameras(CameraIp(ipsCombo->itemText(item)));
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
