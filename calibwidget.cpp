#include "calibwidget.h"
#include "opencv2/opencv.hpp"
#include "logger.h"
#include <QMessageBox>
#include <sstream>

CalibWidget::CalibWidget(Controller & _controller, QWidget * parent)
    :QWidget(parent),
      controller(_controller) {

    calibController = new CalibController(controller);
    shotButton = new QPushButton("Получить снимок", parent);
    findCornersButton = new QPushButton("Найти углы", parent);
    findCornersButton->setEnabled(false);
    addButton = new QPushButton("Добавить", parent);
    addButton->setEnabled(false);
    deleteButton = new QPushButton("Удалить", parent);
    deleteButton->setEnabled(false);
    writeButton = new QPushButton("Записать", parent);
    writeButton->setEnabled(false);


    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(shotButton);
    buttonsLayout->addWidget(findCornersButton);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(writeButton);
    buttonsLayout->addWidget(sizeLabel);

    for(auto p : positions){
        sizeEdits[p] = new SizeEditWidget();
        buttonsLayout->addWidget(sizeEdits[p]);
    }

    QStringList headers;
    headers.append(QString("файл слева"));
    headers.append(QString("файл справа"));
    headers.append(QString("Расст. м. пл."));
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headers);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(shotButton, SIGNAL(released()), this, SLOT(onShotButton()));
    connect(findCornersButton, SIGNAL (released()), this, SLOT (onFindCornersButton()));
    connect(addButton, SIGNAL (released()), this, SLOT (onAddButton()));
    connect(deleteButton, SIGNAL (released()), this, SLOT (onDeleteButton()));
    connect(writeButton, SIGNAL (released()), this, SLOT (onWriteButton()));

    QWidget::setEnabled(false);
    layout->addLayout(buttonsLayout);
    layout->addWidget(table);
    setLayout(layout);

}

void CalibWidget::setEnabled(){
    if(controller.hasCameras())
        QWidget::setEnabled(true);
}

void CalibWidget::onFindCornersButton(){
    Logger::me->log("Поиск углов шахматной доски");
    try{
       calibController->findChessboardCorners(parseSize());
       findCornersButton->setEnabled(false);
       emit updateChessBoardImage();
    }catch(const std::exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        return;
    }
    Logger::me->log("Поиск углов шахматной доски успешно завершен");
}


void CalibWidget::onDeleteButton(){
    int rowCount = table->rowCount();
    if(rowCount > 0)
        calibController->deleteCorners(--rowCount);
}

void CalibWidget::onWriteButton(){
     try{
            calibController->saveYML();
            calibController->sendYML();
            Logger::me->log("Углы сохранены");

        }catch(const std::exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        Logger::me->log("ошибка при сохранении углов");
         return;
    }

}

QString CalibWidget::generateFileName(Position pos){
    stringstream ss;
    ss << "chessboard_";
    ss << pos;
    ss << "_";
    ss << table->rowCount();
    ss << ".png";
    return QString(ss.str().c_str());
}

void CalibWidget::onAddButton(){
    table->setRowCount(table->rowCount() + 1);

    QTableWidgetItem *itemLeftFileName = new QTableWidgetItem(generateFileName(LEFT));
    itemLeftFileName->setFlags(itemLeftFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 0, itemLeftFileName);

    QTableWidgetItem *itemRightFileName = new QTableWidgetItem(generateFileName(RIGHT));
    itemRightFileName->setFlags(itemRightFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 1, itemRightFileName);

    QTableWidgetItem *itemDist = new QTableWidgetItem();
    itemDist->setFlags(Qt::ItemIsEditable);
    table->setItem(2, table->rowCount(), itemRightFileName);

    //findCornersButton->setEnabled(false);

    Logger::me->log("Углы сохранены");
}


void CalibWidget::onShotButton(){
    Logger::me->log("Загружаю снимки");
    try{
        controller.loadShot();
        emit updateChessBoardImage();
//        lUpdateImage();
        findCornersButton->setEnabled(true);
        Logger::me->log("Загрузка снимков завершена");
    }
    catch(const std::exception & e){
        Logger::me->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}



