#ifndef LOGGER_H
#define LOGGER_H

#include <QListWidget>
#include "position.h"

class Logger : public QListWidget
{
public:
    static Logger* me;

    static void log(const QString & message);

    static void log(const Position p, const QString & message);

    void _log(QString message);
private:

    Logger(QWidget * parent = 0);
    QListWidget * logListWidget;

    friend class MainWindow;
};

#endif // LOGGER_H
