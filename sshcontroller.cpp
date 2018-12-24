#include "sshcontroller.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <QtDebug>
#include <stdio.h>
#include <QThread>

SShController::SShController()
{


}

void SShController::init(const QString & hostname, const QString & username, const QString & password ){
    qDebug() << "init ssh = " << hostname;

    int i, auth_pw = 1;
    struct sockaddr_in sin;
    const char *fingerprint;

    int rc;
    libssh2_struct_stat_size got = 0;

    unsigned long hostaddr = inet_addr(hostname.toStdString().c_str());

#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif
    rc = libssh2_init (0);

    if (rc != 0) {
        throw std::runtime_error("libssh2 initialization failed (%d)" + std::to_string( rc));
    }

    /* Ultra basic "connect to port 22 on localhost"
     * Your code is responsible for creating the socket establishing the
     * connection
     */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(sock, (struct sockaddr*)(&sin),
            sizeof(struct sockaddr_in)) != 0) {
        throw std::runtime_error("failed to connect!\n");
    }

    /* Create a session instance
     */
    session = libssh2_session_init();

    if(!session)
        throw std::runtime_error("unable to init ssh session");

    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */
    rc = libssh2_session_handshake(session, sock);

    if(rc) {
        throw std::runtime_error("Failure establishing SSH session: "+ std::to_string(rc));
    }

    /* At this point we havn't yet authenticated.  The first thing to do
     * is check the hostkey's fingerprint against our known hosts Your app
     * may have it hard coded, may go to a file, may present it to the
     * user, that's your call
     */
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    fprintf(stderr, "Fingerprint: ");
    for(i = 0; i < 20; i++) {
        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    }
    fprintf(stderr, "\n");

    if (auth_pw) {
        /* We could authenticate via password */
        if (libssh2_userauth_password(session, username.toStdString().c_str(), password.toStdString().c_str())) {

            throw std::runtime_error("Authentication by password failed.");
        }
    } else {
        /* Or by public key */
        if (libssh2_userauth_publickey_fromfile(session, username.toStdString().c_str(),

                            "/home/username/.ssh/id_rsa.pub",
                            "/home/username/.ssh/id_rsa",
                            password.toStdString().c_str())) {
            fprintf(stderr, "\tAuthentication by public key failed\n");
        }
    }

}

void SShController::command(const QString & command){
    qDebug() << "command = " << command;

    if(session == NULL){
        throw std::runtime_error("ssh session is not opened");
    }
    LIBSSH2_CHANNEL * channel = libssh2_channel_open_session(session);

    int rc = libssh2_channel_exec(channel, command.toStdString().c_str());
    if( rc != 0 )
        {
            throw std::runtime_error("could not open ssh channel");
        }

    libssh2_channel_free(channel);
}

void SShController::fileFrom(const QString & src, const QString & dst){
    qDebug() << "file: " << src << " " << dst;

    if(session == NULL){
        throw std::runtime_error("ssh session is not opened");
    }

    libssh2_struct_stat fileinfo;
    libssh2_struct_stat_size got = 0;
    int rc;

    /* Request a file via SCP */
    LIBSSH2_CHANNEL * channel = libssh2_scp_recv2(session, src.toStdString().c_str(), &fileinfo);

    if (!channel) {
        libssh2_channel_free(channel);
        throw std::runtime_error("Unable to open a channel: " + std::to_string(
                                        libssh2_session_last_errno(session)));
    }

    if( fileinfo.st_size <= 0){
        throw std::runtime_error("Файл на raspberry не существует");
    }

    FILE * pFile;
    pFile = std::fopen (dst.toStdString().c_str(), "wb");


    while(got < fileinfo.st_size) {
            char mem[1024];
            int amount=sizeof(mem);

            if((fileinfo.st_size -got) < amount) {
                amount = (int)(fileinfo.st_size -got);
            }

            rc = libssh2_channel_read(channel, mem, amount);

            if(rc > 0) {
                fwrite(mem, sizeof(char), sizeof(mem), pFile);;
            }
            else if(rc < 0) {
                fclose (pFile);
                libssh2_channel_free(channel);
                throw std::runtime_error("libssh2_channel_read() failed: " + rc);
            }
            got += rc;
        }

    fclose (pFile);
    libssh2_channel_free(channel);
}

void SShController::fileTo(const QString & src, const QString & dst){
    qDebug() << "file: " << src << " " << dst;

    if(session == NULL){
        throw std::runtime_error("ssh session is not opened");
    }

    size_t nread;
    struct stat fileinfo;
    char mem[1024];
    FILE *local;
    char *ptr;
    int rc;

    local = fopen(src.toStdString().c_str(), "rb");
        if (!local) {
            throw std::runtime_error("Can't open local file " + src.toStdString());
        }

    LIBSSH2_CHANNEL *  channel = libssh2_scp_send(session, dst.toStdString().c_str(), fileinfo.st_mode & 0777,

                                   (unsigned long)fileinfo.st_size);

        if (!channel) {
            libssh2_channel_free(channel);
            throw std::runtime_error("Unable to open a channel: " + std::to_string(
                                            libssh2_session_last_errno(session)));
        }

        fprintf(stderr, "SCP session waiting to send file\n");
        do {
            nread = fread(mem, 1, sizeof(mem), local);
            if (nread <= 0) {
                /* end of file */
                break;
            }
            ptr = mem;

            do {
                /* write the same data over and over, until error or completion */
                rc = libssh2_channel_write(channel, ptr, nread);

                if (rc < 0) {
                    throw std::runtime_error("ERROR %d\n");
                    break;
                }
                else {
                    /* rc indicates how many bytes were written this time */
                    ptr += rc;
                    nread -= rc;
                }
            } while (nread);

        } while (1);

        fprintf(stderr, "Sending EOF\n");
        libssh2_channel_send_eof(channel);


        fprintf(stderr, "Waiting for EOF\n");
        libssh2_channel_wait_eof(channel);


        fprintf(stderr, "Waiting for channel to close\n");
        libssh2_channel_wait_closed(channel);


        libssh2_channel_free(channel);
}

void SShController::shutdown(){
    if(session == NULL){
        throw std::runtime_error("ssh session is not opened");
    }

    QThread::sleep(1);

    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");

    libssh2_session_free(session);

    session = 0;

#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    fprintf(stderr, "all done\n");

    libssh2_exit();


    return;
}
