#include "settingswidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include "logger.h"
#include <QMessageBox>
#include <QFileDialog>

SettingsWidget::SettingsWidget(Controller & _controller):
    controller(_controller), QWidget()
{
    QPushButton* saveROIButton = new QPushButton("сохр ROI");
    QPushButton* loadImageButton = new QPushButton("Загрузить изображение");
    shotButton = new QPushButton("Получить снимок");

    QLineEdit* resolutionEdit = new QLineEdit();
    resolutionEdit->setInputMask("999\:999");
    resolutionEdit->setText("400:300");
    resolutionEdit->setFixedWidth(70);
    resolutionEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(loadImageButton);
    layout->addWidget(shotButton);
    layout->addWidget(saveROIButton);
    layout->addWidget(resolutionEdit);


    connect(saveROIButton, SIGNAL (released()), this, SLOT (onSaveROIButton()));
    connect(resolutionEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onResolutionEdit(const QString &)));
    connect(loadImageButton, SIGNAL(released()), this, SLOT(openFile()));
    connect(shotButton, SIGNAL(released()), this, SLOT(onShotButton()));

    saveROIButton->setEnabled(false);
    resolutionEdit->setEnabled(false);
    shotButton->setEnabled(false);
    setLayout(layout);
}

void SettingsWidget::setEnabled(){
    if(controller.hasCameras())
        QWidget::setEnabled(true);
}

void SettingsWidget::onSaveROIButton(){
    Logger::log("Сохраняю область на левое устройство");
    try{
        controller.saveROI();
       Logger::log("Область сохранена");
    }catch(const std::exception & e){
        QMessageBox::warning(this, "error", e.what());
        Logger::log(QString("Область не сохранена. Ошибка: ") + e.what());
    }
    Logger::log("Область сохранена");
}

void SettingsWidget::onResolutionEdit(){

}

void SettingsWidget::openFile(){
    Logger::log("Загрузить изображение");
    QFileDialog fileOpenDialog(this);
    fileOpenDialog.setWindowTitle(tr("Выберите файл с изображением с левой камеры"));
    fileOpenDialog.setViewMode(QFileDialog::Detail);
    fileOpenDialog.setFileMode(QFileDialog::AnyFile);
    fileOpenDialog.setOption(QFileDialog::DontUseNativeDialog);
    fileOpenDialog.setNameFilter(tr("Изображение (*.png)"));

    if(fileOpenDialog.exec()){
        controller.images[LEFT] = fileOpenDialog.selectedFiles()[0];
    }else{
        Logger::log("Загрузка изображения отменена");
        return;
    }

    fileOpenDialog.setWindowTitle(tr("Выберите файл с изображением с правой камеры"));
    if(fileOpenDialog.exec()){
        controller.images[RIGHT] = fileOpenDialog.selectedFiles()[0];
    }else{
        Logger::log("Загрузка изображения отменена");
        return;
    }

    emit updateChessBoardImage();
    Logger::log("Загрузить изображение успешно завершено");
}

void SettingsWidget::onShotButton(){
    Logger::me->log("Загружаю снимки");
    try{
        controller.loadShot();
        emit updateChessBoardImage();
//        lUpdateImage();
        Logger::me->log("Загрузка снимков завершена");
    }
    catch(const std::exception & e){
        Logger::me->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}
