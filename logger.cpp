#include "logger.h"

Logger* Logger::me = NULL;

Logger::Logger(QWidget * parent):
    logListWidget(new QListWidget(parent))
{
    me = this;
    logListWidget->setMaximumHeight(100);
}

void Logger::log(QString message){
    QListWidgetItem* item = new QListWidgetItem(message);
    logListWidget->addItem(item);
    logListWidget->setCurrentItem(item);
}

void Logger::logSt(const QString & message){
    me->log(message);
}
