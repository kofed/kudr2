#include "logger.h"

Logger::Logger(QWidget * parent):
    logListWidget(new QListWidget(parent))
{

}

void Logger::log(QString message){
    QListWidgetItem* item = new QListWidgetItem(message);
    logListWidget->addItem(item);
    logListWidget->setCurrentItem(item);
}
