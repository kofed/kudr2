#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include "pngwidget.h"
#include <QMessageBox>
#include <QTableWidget>
#include "camerasearchwidget.h"
#include "settingswidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(logger)
{
    ui->setupUi(this);

    logger = new Logger;

    CameraSearchWidget *searchWidget = new CameraSearchWidget(controller);
    CalibWidget *calibWidget = new CalibWidget(controller);

    QVBoxLayout* settingsLayout = new QVBoxLayout();
    ui->settingsWidget->setLayout(settingsLayout);
    settingsLayout->addWidget(searchWidget);
    SettingsWidget* settingsWidget = new SettingsWidget(controller);
    settingsLayout->addWidget(settingsWidget);
    settingsLayout->addWidget(calibWidget);
    settingsLayout->addWidget(logger);

    QVBoxLayout* shotLayout = new QVBoxLayout();
    ui->shotWidget->setLayout(shotLayout);

    ShotWidget *shotWidget = new ShotWidget(controller);
    shotLayout->addWidget(shotWidget);

    connect(searchWidget, SIGNAL(ipSelected()), calibWidget, SLOT(setEnabled()));
    connect(searchWidget, SIGNAL(ipSelected()), settingsWidget, SLOT(setEnabled()));
    connect(searchWidget, SIGNAL(ipSelected()), shotWidget, SLOT(updateIpLabels()));
    connect(calibWidget, SIGNAL(updateChessBoardImage()), shotWidget, SLOT(update()));

    //ui->horizontalLayout_2->addWidget(saveShotButton);

    //update();

    Logger::log("Добро пожаловать!");

}

void MainWindow::update(){
    bool isLNull = controller.cameras[LEFT] == NULL;
    bool isRNull = controller.cameras[RIGHT] == NULL;
    bool isNull = isLNull || isRNull;

    /*settingsWidget->setEnabled(!isNull);

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
*/
}



MainWindow::~MainWindow()
{
    delete logger;
    delete ui;
}

void MainWindow::onIpchanged(const QString & ip){
    try{
        controller.setCameras(CameraIp(ip));
        update();
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
    }
}


void MainWindow::onRResolutionEdit(const QString &){
    /*QString sResolution = ui->rResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.rWidth = QString(slResolution.at(0)).toInt();
    controller.rHeight = QString(slResolution.at(1)).toInt();
*/
}

void MainWindow::onLResolutionEdit(const QString &){
    /*QString sResolution = ui->lResolutionEdit->text();
    QStringList slResolution = sResolution.split(":");
    controller.lWidth = QString(slResolution.at(0)).toInt();
    controller.lHeight = QString(slResolution.at(1)).toInt();
*/
}
