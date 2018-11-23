#ifndef LOGGER_H
#define LOGGER_H

#include <QListWidget>

class Logger
{

public:
     QListWidget * logListWidget;

    Logger(QWidget * parent);

    void log(QString message);
};

#endif // LOGGER_H
