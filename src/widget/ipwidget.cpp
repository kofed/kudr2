#include "ipwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "position.h"
#include <QMessageBox>

IpWidget::IpWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    QPushButton* saveButton = new QPushButton("Сохр. номер двери");

    doorNumEdit->setValidator( new QIntValidator(1, 17, this) );

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(doorNumEdit);
    layout->addWidget(lCombo);
    layout->addWidget(rCombo);
    layout->addWidget(saveButton);

    lCombo->addItem("Левая", LEFT);
    lCombo->addItem("Правая", RIGHT);

    rCombo->addItem("Левая", LEFT);
    rCombo->addItem("Правая", RIGHT);

    rCombo->setCurrentIndex(RIGHT);

    connect(saveButton, SIGNAL (released()), this, SLOT (onSaveDoorNumButton()));

    setLayout(layout);
}

void IpWidget::onSaveDoorNumButton(){
    Logger::log("Сохраняю номер двери и позицию");
        int doorNumber = doorNumEdit->text().toInt();

        Position lPos = lCombo->currentData().value<Position>();
        CameraIp newLeftCamera(doorNumber, lPos);

        Position rPos = rCombo->currentData().value<Position>();
        CameraIp newRightCamera(doorNumber, rPos);

        Logger::log("... на левую камеру");
        try{
            controller.saveCamera(LEFT, newLeftCamera);
            Logger::log("... успешно завершено");
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
            Logger::log(QString("... произошла ошибка ") + e.what());
        }
        Logger::log("на правую камеру");
        try{
            controller.saveCamera(RIGHT, newRightCamera);
            Logger::log("... успешно завершено");
        }
        catch(const std::exception & e){
            QMessageBox::warning(this, "error", e.what());
            Logger::log(QString("... произошла ошибка ") + e.what());
        }
       // update();
}
