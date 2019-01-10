#include <QVBoxLayout>
#include <QHBoxLayout>
#include "shotwidget.h"
#include <QMessageBox>
#include <QApplication>

ShotWidget::ShotWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    pngLabels.insert(std::pair<Position, QLabel*> (LEFT, new QLabel));
    pngLabels[RIGHT] = new QLabel();

    QVBoxLayout * layout = new QVBoxLayout();
    QHBoxLayout * labelLayout = new QHBoxLayout();
    labelLayout->addWidget(new QLabel("камера слева"));
    labelLayout->addWidget(new QLabel("камера справа"));
    layout->addLayout(labelLayout);

    QHBoxLayout * pngLayout = new QHBoxLayout();
    layout->addLayout(pngLayout);
    pngLayout->addWidget(pngLabels[LEFT]);
    pngLayout->addWidget(pngLabels[RIGHT]);

    QHBoxLayout * ipLayout = new QHBoxLayout();
    for(auto p : positions){
        ipLabels[p] = new QLabel;
        ipLayout->addWidget(ipLabels[p]);
    }
    layout->addLayout(ipLayout);

    setLayout(layout);

    initPngWidgets();
}

void ShotWidget::initPngWidgets(){
    for(auto p : positions){
        pngWidgets[p] = new PngWidget(pngLabels[p]);
    }
}

void ShotWidget::update(){
    for(auto p : positions){
       if(controller.cameras[p] == NULL)
            continue;
        try{
            QString imageFile = controller.getImgFileName(p);
            QPixmap pixmap(imageFile);

            pngLabels[p]->setPixmap(pixmap);
            pngWidgets[p]->resize(pixmap.size());
            pngWidgets[p]->show();
           QApplication::processEvents();
            emit imageUpdated();
        }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
    }
}

void ShotWidget::updateIpLabels(){
    for(auto p : positions){
        CameraIp* cameraIp = controller.cameras[p];
        if(cameraIp != NULL)
            ipLabels[p]->setText(cameraIp->toString());
    }
}
