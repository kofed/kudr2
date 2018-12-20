#include "calibwidget.h"
#include "opencv2/opencv.hpp"
#include "logger.h"
#include <QMessageBox>

CalibWidget::CalibWidget( QWidget* parent,
                          QTableWidget * _lTable,
                          QTableWidget * _rTable,
                          Controller & _controller)
    :QWidget(parent), lTable(_lTable), rTable(_rTable),
      controller(_controller) {

    shotButton = new QPushButton("Получить снимок", parent);
    findCornersButton = new QPushButton("Найти углы", parent);
    addButton = new QPushButton("Добавить", parent);
    deleteButton = new QPushButton("Удалить", parent);
    writeButton = new QPushButton("Записать", parent);
    sizeEdit->setInputMask("99\:99");

    setLayout(mainLayout);
    mainLayout->addWidget(shotButton);
    mainLayout->addWidget(findCornersButton);
    mainLayout->addWidget(addButton);
    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(writeButton);
    mainLayout->addWidget(sizeLabel);
    mainLayout->addWidget(sizeEdit);

    QStringList headers;
    headers.append(QString("файл"));
    headers.append(QString("Расст. м. пл."));
    lTable->setColumnCount(2);
    lTable->setHorizontalHeaderLabels(headers);

    rTable->setColumnCount(2);
    rTable->setHorizontalHeaderLabels(headers);

    connect(shotButton, SIGNAL(released()), this, SLOT(onShotButton()));
    connect(findCornersButton, SIGNAL (released()), this, SLOT (onFindCornersButton()));
    connect(addButton, SIGNAL (released()), this, SLOT (onAddButton()));
    connect(deleteButton, SIGNAL (released()), this, SLOT (onDeleteButton()));
    connect(writeButton, SIGNAL (released()), this, SLOT (onWriteButton()));

    setEnabled(false);
}

void CalibWidget::onFindCornersButton(){


    Logger::me->log("Поиск углов шахматной доски");
    try{
       for(auto p : positions){
            string fname = controller.getImgFileName(p).toStdString();
            Mat image = imread(fname);
            if(!image.data){
                Logger::me->log("Отсутствует файл с изображением шахматной доски");
                continue;
            }
            calibController->findChessboardCorners(p, image, parseSize());
            imwrite(fname, image);
        }
       findCornersButton->setEnabled(false);
    }catch(const std::exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        return;
    }

    emit updateChessBoardImage();
    Logger::me->log("Поиск углов шахматной доски успешно завершен");
}

void CalibWidget::onAddButton(){
    //calibController->addCorners();

    int pos = lTable->rowCount();
    lTable->insertRow(pos);

    lTable->setItem(pos, 0, new QTableWidgetItem("/tmp/1.png"));


   /* Qt::ItemFlags flags = lTable->item(1, pos)->flags();
    flags |= Qt::ItemIsEditable;
    lTable->item(1, pos)->setFlags(flags);

    rTable->insertRow(rTable->rowCount());

    findCornersButton->setEnabled(false);*/

    Logger::me->log("Углы сохранены");
}

void CalibWidget::onDeleteButton(){
    int rowCount = lTable->rowCount();
    if(rowCount > 0)
        calibController->deleteCorners(--rowCount);
}

void CalibWidget::onWriteButton(){
    calibController->saveYML();
}

void CalibWidget::listChessboardImages(const QString & name) const{
    //Use opencv yml

    /*for (int i=0;i< ui->tableWidget->rowCount();i++) {
            for (int j=0;j< ui->tableWidget->columnCount();j++) {
                QTableWidgetItem *item =  ui->tableWidget->item(i,j);
                item->setFlags(Qt::NoItemFlags);
            }
        }*/
}

Size CalibWidget::parseSize(){
    QStringList sl = sizeEdit->text().split(":");
    if(sl.size() != 2){
        throw std::runtime_error("заполните размер шахматной доски");
    }
    return Size(sl.at(0).toInt(), sl.at(1).toInt());
}

void CalibWidget::onShotButton(){
    Logger::me->log("Загружаю снимок с левого устройства");
    try{
        controller.loadShot(controller.rWidth, controller.rHeight);
//        lUpdateImage();
        Logger::me->log("Загрузка снимка успешно завершена");
    }
    catch(const std::exception & e){
        Logger::me->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}

