#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller
{
    enum CameraPosition {left, right};

public:
    Controller();

    void saveROI(const CameraPosition position);

    void loadDebug(const CameraPosition position);

    void loadShot(const CameraPosition position);

    void cameraOn(const CameraPosition position);
};

#endif // CONTROLLER_H
