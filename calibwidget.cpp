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
    //addButton->setEnabled(false); UNCOMMENT THIS LINE
    deleteButton = new QPushButton("Удалить", parent);
    deleteButton->setEnabled(false);
    writeButton = new QPushButton("Записать", parent);
    writeButton->setEnabled(false);

    QLabel* hLabel = new QLabel("Высота");
    hEdit->setInputMask("999");;


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
    buttonsLayout->addWidget(hLabel);
    buttonsLayout->addWidget(hEdit);

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

  //  QWidget::setEnabled(false); UNCOMMENT THIS LINE
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
        for(auto p : positions){
            calibController->sizes[p] = sizeEdits[p]->getSize();
        }

       calibController->findChessboardCorners();
       findCornersButton->setEnabled(false);
       emit updateChessBoardImage();
    }catch(const std::exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        return;
    }
    Logger::me->log("Поиск углов шахматной доски успешно завершен");
}


void CalibWidget::onDeleteButton(){
    /*int rowCount = table->rowCount();
    if(rowCount > 0)
        calibController->deleteCorners(--rowCount);*/
}

void CalibWidget::onWriteButton(){
     try{




        calibController->saveYML();
  //          calibController->sendYML(); UNCOMMENT THIS LINE
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
    int h;
    try{
        h = hEdit->text().toInt();
    }catch(exception & e){
        QMessageBox::warning(this, "Ошибка", "Укажите расстояние между плоскостями");
        Logger::log("Укажите расстояние между плоскостями");
        return;
    }
    table->setRowCount(table->rowCount() + 1);

    QTableWidgetItem *itemLeftFileName = new QTableWidgetItem(generateFileName(LEFT));
//    itemLeftFileName->setFlags(itemLeftFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 0, itemLeftFileName);

    QTableWidgetItem *itemRightFileName = new QTableWidgetItem(generateFileName(RIGHT));
//    itemRightFileName->setFlags(itemRightFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 1, itemRightFileName);

    QTableWidgetItem* itemH = new QTableWidgetItem(h);
    table->setItem(table->rowCount() - 1, 2, itemH);

    QTableWidgetItem* itemSizeLeft = new QTableWidgetItem(calibController->sizes[LEFT]);
    table->setItem(table->rowCount() - 1, 3, itemSizeLeft);

    QTableWidgetItem* itemSizeRight = new QTableWidgetItem(calibController->sizes[RIGHT]);
    table->setItem(table->rowCount() - 1, 4, itemSizeRight);

    calibController->addCalibEntities(calibController->centers[LEFT], calibController->centers[RIGHT], h);
    writeButton->setEnabled(true);

    Logger::me->log("Углы добавлены. Укажите расстояние между плоскостями");
}


void CalibWidget::onShotButton(){
    Logger::me->log("Загружаю снимки");
    try{
        controller.loadShot();
        emit updateChessBoardImage();
//        lUpdateImage();
        findCornersButton->setEnabled(true);
        addButton->setEnabled(true);
        Logger::me->log("Загрузка снимков завершена");
    }
    catch(const std::exception & e){
        Logger::me->log(QString("Не удалось загрузить снимок с устройства. Ошибка ") + e.what());
        QMessageBox::warning(this, "error", e.what());
    }
}



