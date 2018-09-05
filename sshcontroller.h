#ifndef SSHCONTROLLER_H
#define SSHCONTROLLER_H

#include <QString>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <libssh2.h>

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
//#ifdef HAVE_UNISTD_H
#include <unistd.h>
//#endif
#include <arpa/inet.h>
//#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
//#endif

class SShController
{
public:
    SShController();

    void init(const QString & hostname, const QString & name = "pi", const QString & password = "raspberry" );
    void command(const QString & command);

    void shutdown();

private:
    LIBSSH2_SESSION *session;

    int sock;
};

#endif // SSHCONTROLLER_H
