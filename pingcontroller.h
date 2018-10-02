#ifndef PINGCONTROLLER_H
#define PINGCONTROLLER_H

#include <set>
#include <QProcess>


class PingController
{
    const int MAX_DOOR = 8;

public:
    PingController();

    std::set<QString> activeIps;

    int ping(const QString & ip);
    void scan();
};

#endif // PINGCONTROLLER_H
