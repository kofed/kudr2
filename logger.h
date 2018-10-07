#ifndef LOGGER_H
#define LOGGER_H

#include <QListWidget>

class Logger
{
    QListWidget * logListWidget;
public:
    Logger(QListWidget * logListWidget);

    void log(QString message);
};

#endif // LOGGER_H
