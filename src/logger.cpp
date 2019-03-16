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

void Logger::log(const Position p, const QString & message){
    if(p == LEFT){
        log(QString("Камера слева: ") + message);
    }else{
        log(QString("Камера справа: ") + message);
    }
}
