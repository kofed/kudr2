#include "camerasearchwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "cameraip.h"

CameraSearchWidget::CameraSearchWidget(Controller & _controller, QWidget *parent):
    controller(_controller), QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(searchButton);
    layout->addWidget(new QLabel("по номеру двери"));
    layout->addWidget(doorNumEdit);
    layout->addWidget(ipsCombo);
    layout->setAlignment(Qt::AlignTop);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,1);
    layout->setStretch(3,1);
    setLayout(layout);

    connect(searchButton, SIGNAL(released()), this, SLOT(onSearchButton()));
    connect(ipsCombo, SIGNAL(activated(int)), this, SLOT(onIpsComboSelected(int )));

}

void CameraSearchWidget::onSearchButton(){
    Logger::log("поиск устройств");
    try{
        QString doorNum = doorNumEdit->text();
        if(doorNum.isEmpty()){
            pingController.scan();
            initIpsCombo();
        }else{
            CameraIp cam = CameraIp(doorNum.toInt(), LEFT);
            controller.setCameras(cam);
            emit ipSelected();
        }

    }catch(std::exception & e){
        QMessageBox::warning(this, "ошибка при поиске устройств", e.what());
        return;
    }
    Logger::log("поиск устройств завершен");
}

void CameraSearchWidget::onIpsComboSelected(int idx){
    if(!ipsCombo->currentText().isEmpty()){
        controller.setCameras(CameraIp(ipsCombo->currentText()));
        emit ipSelected();
    }
}

void CameraSearchWidget::initIpsCombo(){
    ipsCombo->clear();
    for(auto i : pingController.activeIps){
        ipsCombo->addItem(i);
    }
}

