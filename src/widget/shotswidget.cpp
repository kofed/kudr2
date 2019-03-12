#include <QVBoxLayout>
#include <QHBoxLayout>
#include "shotswidget.h"
#include <QMessageBox>
#include <QApplication>
#include <QImage>

ShotsWidget::ShotsWidget(Controller & _controller, CalibController & _calibController):QWidget(),
    controller(_controller), calibController(_calibController)
{
    for(auto p : positions){
        pngWidgets[p] = new ShotWidget(p, _calibController);
    }

    QVBoxLayout * layout = new QVBoxLayout();
    QHBoxLayout * labelLayout = new QHBoxLayout();
    labelLayout->addWidget(new QLabel("камера слева"));
    labelLayout->addWidget(new QLabel("камера справа"));
    layout->addLayout(labelLayout);

    QHBoxLayout * pngLayout = new QHBoxLayout();
    layout->addLayout(pngLayout);
    pngLayout->addWidget(pngWidgets[LEFT]);
    pngLayout->addWidget(pngWidgets[RIGHT]);

    QHBoxLayout * ipLayout = new QHBoxLayout();
    for(auto p : positions){
        ipLabels[p] = new QLabel;
        ipLayout->addWidget(ipLabels[p]);
    }
    layout->addLayout(ipLayout);

    setLayout(layout);
}

void ShotsWidget::update(){
    for(auto p : positions){
        try{
            pngWidgets[p]->update();

        }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
    }
    emit imageUpdated();
}

void ShotsWidget::init(){
    for(auto p : positions){
        try{
            pngWidgets[p]->init();

        }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
    }
    emit imageUpdated();
}

void ShotsWidget::updateIpLabels(){
    for(auto p : positions){
        CameraIp* cameraIp = controller.cameras[p];
        if(cameraIp != NULL)
            ipLabels[p]->setText(cameraIp->toString());
    }
}
