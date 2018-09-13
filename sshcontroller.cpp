#include "sshcontroller.h"



SShController::SShController()
{


}

void SShController::init(const QString & hostname, const QString & username, const QString & password ){
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
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return;
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
        fprintf(stderr, "failed to connect!\n");
        return;
    }

    /* Create a session instance
     */
    session = libssh2_session_init();

    if(!session)
        return;

    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */
    rc = libssh2_session_handshake(session, sock);

    if(rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return;
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

            fprintf(stderr, "Authentication by password failed.\n");
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
    LIBSSH2_CHANNEL * channel = libssh2_channel_open_session(session);

    int rc = libssh2_channel_exec(channel, command.toStdString().c_str());
    if( rc != 0 )
        {
            fprintf(stderr,"Error\n");
            exit( 1 );
        }

    libssh2_channel_free(channel);
}

void SShController::file(const QString & src, const QString & dst){
    /* Request a file via SCP */
    LIBSSH2_CHANNEL * channel = libssh2_scp_recv2(session, src.toStdString().c_str(), &fileinfo);

    if (!channel) {
        fprintf(stderr, "Unable to open a session: %d\n",
                libssh2_session_last_errno(session));
    }

    libssh2_channel_free(channel);
}

void SShController::shutdown(){

    libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");

    libssh2_session_free(session);

#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    fprintf(stderr, "all done\n");

    libssh2_exit();


    return;
}
