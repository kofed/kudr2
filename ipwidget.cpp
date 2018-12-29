#include "ipwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "position.h"

IpWidget::IpWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    QComboBox * lCombo = new QComboBox;
    QComboBox * rCombo = new QComboBox;
    QPushButton* saveButton = new QPushButton("Сохр. номер двери");

    QLineEdit* doorNumEdit = new QLineEdit();
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
       // update();
}
