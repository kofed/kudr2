#include "calibwidget.h"
#include "opencv2/opencv.hpp"
#include <QStringBuilder>

CalibWidget::CalibWidget(const QTableWidget *_table, const QPushButton * _saveShotButton)
    :table(_table), saveShotButton(_saveShotButton) {
    QStringList headers;
    headers.append(QString("файл"));
    headers.append(QString("Расст. м. пл."));
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(headers);

    connect(saveShotButton, SIGNAL (released()), this, SLOT (onLShotButton()));
}

CalibWidget::~CalibWidget(){

}

QString generateFileName(){
    QStringBuilder name("chessboard_");
    name + "left_";
    name + table.rowCount();
    return name();
}

void CalibWidget::onSaveShotButton(){
    QTableWidgetItem *itemFileName = new QTableWidgetItem(generateFileName());
    table->setItem(COLUMN_FILE_NAME, table.rowCount(), itemFileName);
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
