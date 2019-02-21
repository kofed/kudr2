#include "logger.h"

Logger* Logger::me = NULL;

Logger::Logger(QWidget * parent):
    QListWidget(parent)
{
    me = this;
    setMaximumHeight(100);
}

void Logger::_log(QString message){
    QListWidgetItem* item = new QListWidgetItem(message);
    addItem(item);
    setCurrentItem(item);
}

void Logger::log(const QString & message){
    me->_log(message);
}
