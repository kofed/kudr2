#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QTableWidget>
#include "camerasearchwidget.h"
#include "settingswidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller()
{
    ui->setupUi(this);

    logger = new Logger;

    CameraSearchWidget *searchWidget = new CameraSearchWidget(controller);
    CalibWidget *calibWidget = new CalibWidget(controller);

    QVBoxLayout* settingsLayout = new QVBoxLayout();
    ui->settingsWidget->setLayout(settingsLayout);
    settingsLayout->addWidget(searchWidget);
    SettingsWidget* settingsWidget = new SettingsWidget(controller, *(calibWidget->calibController));
    settingsLayout->addWidget(settingsWidget);
    settingsLayout->addWidget(calibWidget);
    settingsLayout->addWidget(logger);

    QVBoxLayout* shotLayout = new QVBoxLayout();
    ui->shotsWidget->setLayout(shotLayout);

    ShotsWidget *shotsWidget = new ShotsWidget(controller, *(calibWidget->calibController));
    shotLayout->addWidget(shotsWidget);

    connect(settingsWidget, SIGNAL(updateChessBoardImage()), calibWidget, SLOT(setEnabled()));
    connect(settingsWidget, SIGNAL(updateChessBoardImage()), shotsWidget, SLOT(update()));
    connect(searchWidget, SIGNAL(ipSelected()), settingsWidget, SLOT(setEnabled()));
    connect(searchWidget, SIGNAL(ipSelected()), shotsWidget, SLOT(updateIpLabels()));
    connect(calibWidget, SIGNAL(updateChessBoardImage()), shotsWidget, SLOT(update()));

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

