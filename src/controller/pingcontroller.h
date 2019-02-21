#ifndef PINGCONTROLLER_H
#define PINGCONTROLLER_H

#include <set>
#include <QProcess>
#include "logger.h"


class PingController
{
    const int MAX_DOOR = 8;

    Logger* logger;
public:
    PingController();

    std::set<QString> activeIps;

    int ping(const QString & ip);
    void scan();
};

#endif // PINGCONTROLLER_H
