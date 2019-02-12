#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H
#include <QWidget>
#include <QLabel>
#include <map>
#include "pngwidget.h"
#include "controller.h"
#include "position.h"
#include <utility>
#include "imageviewer.h"

using namespace std;

class ShotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShotWidget(Controller & _controller, CalibController & _calibController);

    virtual ~ShotWidget(){}

signals:
    void imageUpdated();

public slots:
    void updateIpLabels();
    void update();

private:
    Controller & controller;
    CalibController & calibController;
    map<Position, ImageViewer*> pngWidgets;
    map<Position, QLabel*> ipLabels;


    void initPngWidgets();

};

#endif // SHOTWIDGET_H
