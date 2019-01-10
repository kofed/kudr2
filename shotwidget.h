#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H
#include <QWidget>
#include <QLabel>
#include <map>
#include "pngwidget.h"
#include "controller.h"
#include "position.h"
#include <utility>

using namespace std;

class ShotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShotWidget(Controller & _controller);

    virtual ~ShotWidget(){}

signals:
    void imageUpdated();

public slots:
    void updateIpLabels();
    void update();

private:
    Controller & controller;
    map<Position, QLabel*> pngLabels;
    map<Position, PngWidget*> pngWidgets;
    map<Position, QLabel*> ipLabels;


    void initPngWidgets();

};

#endif // SHOTWIDGET_H
