#include "logger.h"

Logger::Logger(QListWidget * _logListWidget):
    logListWidget(_logListWidget)
{

}

void Logger::log(QString message){
    QListWidgetItem* item = new QListWidgetItem(message);
    logListWidget->addItem(item);
    logListWidget->setCurrentItem(item);
}
