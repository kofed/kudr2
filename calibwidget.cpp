#include "calibwidget.h"
#include "ui_calibwidget.h"
#include "opencv2/opencv.hpp"

CalibWidget::CalibWidget( QTableWidget *_table):table(_table) {
    QStringList headers;
    headers.append(QString("файл"));
    headers.append(QString("Расст. м. пл."));
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(headers);


}

CalibWidget::~CalibWidget(){

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
