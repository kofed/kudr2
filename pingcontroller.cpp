#include "pingcontroller.h"
#include "CameraIp.h"
#include <QtDebug>

PingController::PingController()
{

}

int PingController::ping(const QString & ip){
    QProcess * process = new QProcess();
    int result = process->execute("ping", QStringList() << "-c" << "1" << QString(ip));
    delete process;
    return result;

}

void PingController::scan(){

        for(int i = 1; i < MAX_DOOR; ++i ){
            for(int j = Position::LEFT; j != Position::NOTSET; ++j ){
                QString ip = QString("192.168.43.%1%2").arg(i).arg(j);
                if(ping(ip) == 0){
                    activeIps.insert(ip);
                    qDebug() << "pinged " << ip;
                };

            }
        }
   // activeIps.insert("192.168.43.200");
}
