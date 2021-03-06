#include "calibwidget.h"
#include "opencv2/opencv.hpp"
#include "logger.h"
#include <QMessageBox>
#include <sstream>

CalibWidget::CalibWidget(Controller & _controller, QWidget * parent)
    :QWidget(parent),
      controller(_controller) {

    calibController = new CalibController(controller, controller.rois);
    findCornersButton = new QPushButton("Найти углы", parent);
    findCornersButton->setEnabled(true);
    addButton = new QPushButton("Добавить", parent);
  //  addButton->setEnabled(false);
    deleteButton = new QPushButton("Удалить", parent);
  //  deleteButton->setEnabled(false);
    writeButton = new QPushButton("Записать", parent);
 //   writeButton->setEnabled(false);

    QLabel* hLabel = new QLabel("Высота");
    hEdit->setValidator(new QIntValidator());


    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(findCornersButton);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(writeButton);
    buttonsLayout->addWidget(sizeLabel);
    for(auto p : positions){
        sizeEdits[p] = new SizeEditWidget("ш/в");
        buttonsLayout->addWidget(sizeEdits[p]);
    }
    buttonsLayout->addWidget(hLabel);
    buttonsLayout->addWidget(hEdit);
    buttonsLayout->addWidget(new QLabel("размер клетки"));

    buttonsLayout->addWidget(cellSizeEdit);
    cellSizeEdit->setValidator(new QIntValidator(10, 1000, this));
    QStringList headers;
    headers.append(QString("файл слева"));
    headers.append(QString("файл справа"));
    headers.append(QString("Расст. м. пл."));
    headers.append(QString("Размер доски слева"));
    headers.append(QString("Размер доски справа"));
    headers.append(QString("Размер клетки"));
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels(headers);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(findCornersButton, SIGNAL (released()), this, SLOT (onFindCornersButton()));
    connect(addButton, SIGNAL (released()), this, SLOT (onAddButton()));
    connect(deleteButton, SIGNAL (released()), this, SLOT (onDeleteButton()));
    connect(writeButton, SIGNAL (released()), this, SLOT (onWriteButton()));
    connect(sizeEdits[LEFT], SIGNAL(redactionFinished()), this, SLOT(onLeftSizeRedactionFinished()));
    connect(sizeEdits[RIGHT], SIGNAL(redactionFinished()), this, SLOT(onRightSizeRedactionFinished()));


    QWidget::setEnabled(false);
    layout->addLayout(buttonsLayout);
    layout->addWidget(table);
    setLayout(layout);

}

void CalibWidget::setEnabled(){
    QWidget::setEnabled(true);
    findCornersButton->setEnabled(true);
}

void CalibWidget::onFindCornersButton(){
    Logger::me->log("Поиск углов шахматной доски");
    try{
        for(auto p : positions){
            calibController->sizes[p] = sizeEdits[p]->getSize();
        }
       calibController->findChessboardCorners();

    }catch(const std::exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        Logger::me->log(e.what());
        emit updateChessBoardImage();
        return;
    }
    emit updateChessBoardImage();
    addButton->setEnabled(true);
    Logger::me->log("Поиск углов шахматной доски завершен");
}


void CalibWidget::onDeleteButton(){
    calibController->clear();
    deleteButton->setEnabled(false);
    emit updateChessBoardImage();
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
    int h;
    try{
        h = hEdit->text().toInt();
    }catch(exception & e){
        QMessageBox::warning(this, "Ошибка", "Укажите расстояние между плоскостями");
        Logger::log("Укажите расстояние между плоскостями");
        return;
    }


    int cellSize = cellSizeEdit->text().toInt();

    try{
        calibController->addCalibEntities(h, cellSize);
    }catch(exception & e){
        QMessageBox::warning(this, e.what(), e.what());
        Logger::me->log("ошибка при добавлении углов");
        return;
    }


    table->setRowCount(table->rowCount() + 1);

    QTableWidgetItem *itemLeftFileName = new QTableWidgetItem(generateFileName(LEFT));
//    itemLeftFileName->setFlags(itemLeftFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 0, itemLeftFileName);

    QTableWidgetItem *itemRightFileName = new QTableWidgetItem(generateFileName(RIGHT));
//    itemRightFileName->setFlags(itemRightFileName->flags() ^ Qt::ItemIsEditable);
    table->setItem(table->rowCount() - 1, 1, itemRightFileName);

    QTableWidgetItem* itemH = new QTableWidgetItem(hEdit->text());
    table->setItem(table->rowCount() - 1, 2, itemH);

    QTableWidgetItem* itemSizeLeft = new QTableWidgetItem(sizeEdits[LEFT]->text());
    table->setItem(table->rowCount() - 1, 3, itemSizeLeft);

    QTableWidgetItem* itemSizeRight = new QTableWidgetItem(sizeEdits[RIGHT]->text());
    table->setItem(table->rowCount() - 1, 4, itemSizeRight);

    QTableWidgetItem* itemCellSize = new QTableWidgetItem(cellSizeEdit->text());
    table->setItem(table->rowCount() - 1, 5, itemCellSize);

    hEdit->clear();
    for(auto p : positions){
        sizeEdits[p]->clear();
    }
    writeButton->setEnabled(true);
    addButton->setEnabled(false);

    Logger::me->log("Углы добавлены. Укажите расстояние между плоскостями");
}

void CalibWidget::onLeftSizeRedactionFinished(){
    calibController->sizes[LEFT] = sizeEdits[LEFT]->getSize();
}

void CalibWidget::onRightSizeRedactionFinished(){
    calibController->sizes[RIGHT] = sizeEdits[RIGHT]->getSize();

}

