#include <QVBoxLayout>
#include <QHBoxLayout>
#include "shotwidget.h"

ShotWidget::ShotWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    QLabel l;
    pngLabels.insert(std::pair<Position, QLabel> (LEFT, l));
    pngLabels[RIGHT] = QLabel();

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
    ipLayout->addWidget(lIpLabel);
    ipLayout->addWidget(rIpLabel);
    layout->addLayout(ipLayout);

    setLayout(layout);
}

void MainWindow::updateImage(){
    for(auto p : positions){
       if(controller.cameras[LEFT] == NULL)
            return;
        try{
            QString imageFile = controller.getImgFileName(p);
            QPixmap pixmap(imageFile);
            pngWidgets[p] = PngWidget(pngLabels[p]);
            pngLabels[p]->setPixmap(pixmap);
            pngWidgets[p]->resize(pixmap.size());
            pngWidgets[p]->show();
            qApp->processEvents();
            emit imageUpdated();
        }catch(const std::exception & e){
            QMessageBox::warning(this, "Невозможно найти локальный файл", "Невозможно найти локальный файл. Возможно на устройстве он не был записан.");
        }
    }
}
