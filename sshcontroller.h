#ifndef SSHCONTROLLER_H
#define SSHCONTROLLER_H

#include <string>

using namespace std;


class SShController
{
public:
    SShController(string ip, string name, string password );

    void command(string command);
};

#endif // SSHCONTROLLER_H
