#ifndef LOGGER_H
#define LOGGER_H

#include <QListWidget>

class Logger
{
public:
    static Logger* me;

    static void logSt(const QString & message);

    void log(QString message);
private:

    Logger(QWidget * parent);
    QListWidget * logListWidget;

    friend class MainWindow;
};

#endif // LOGGER_H
