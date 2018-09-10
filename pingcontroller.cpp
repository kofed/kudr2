#include "pingcontroller.h"
#include "CameraIp.h"

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

        for(int i = 10; i < 10 + MAX_DOOR; ++i ){
            for(int j = CameraIp::CameraPosition::left; j != CameraIp::CameraPosition::right; ++j ){
                QString ip = QString("192.168.43.%1%2").arg(i).arg(j);
                if(ping(ip) == 0){
                    activeIps.insert(ip);
                };

            }
        }
}
