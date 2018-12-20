#include "logger.h"

Logger* Logger::me = NULL;

Logger::Logger(QWidget * parent):
    logListWidget(new QListWidget(parent))
{
    me = this;
}

void Logger::log(QString message){
    QListWidgetItem* item = new QListWidgetItem(message);
    logListWidget->addItem(item);
    logListWidget->setCurrentItem(item);
}
