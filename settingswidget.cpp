#include "settingswidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include "logger.h"

SettingsWidget::SettingsWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    QPushButton* saveROIButton = new QPushButton("сохр ROI");
    QLineEdit* resolutionEdit = new QLineEdit();
    resolutionEdit->setInputMask("999\:999");
    resolutionEdit->setText("400:300");
    resolutionEdit->setFixedWidth(70);
    resolutionEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(saveROIButton);
    layout->addWidget(resolutionEdit);

    connect(saveROIButton, SIGNAL (released()), this, SLOT (onSaveROIButton()));
    connect(resolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onResolutionEdit(const QString &)));

    QWidget::setEnabled(false);
    setLayout(layout);
}

void SettingsWidget::setEnabled(){
    if(controller.hasCameras())
        QWidget::setEnabled(true);
}

void SettingsWidget::onSaveROIButton(){
    Logger::log("Сохраняю область на левое устройство");
    /*logger->log("Сохраняю область на левое устройство");
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
    }*/
    Logger::log("Область сохранена");
}

void SettingsWidget::onResolutionEdit(){

}
