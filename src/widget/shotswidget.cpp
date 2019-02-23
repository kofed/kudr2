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
            QPixmap pixmap1 = mat2QPixmap(calibController.images[p]);
            pngWidgets[p]->setImage(pixmap1);
            pngWidgets[p]->resize(pixmap1.size());
            pngWidgets[p]->show();
           QApplication::processEvents();
            emit imageUpdated();
        }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
    }
}

void ShotsWidget::updateIpLabels(){
    for(auto p : positions){
        CameraIp* cameraIp = controller.cameras[p];
        if(cameraIp != NULL)
            ipLabels[p]->setText(cameraIp->toString());
    }
}

QPixmap ShotsWidget::mat2QPixmap(const Mat & image){
    if(!image.data){
        return QPixmap;
    }

    return QPixmap::fromImage(QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_RGB888));
}
