#ifndef ShotsWidget_H
#define ShotsWidget_H
#include <QWidget>
#include <QLabel>
#include <map>
#include "pngwidget.h"
#include "controller.h"
#include "position.h"
#include <utility>
#include "shotswidget.h"
#include "shotswidget.h"
#include "shotwidget.h"

using namespace std;

class ShotsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShotsWidget(Controller & _controller, CalibController & _calibController);

    virtual ~ShotsWidget(){}

signals:
    void imageUpdated();

public slots:
    void updateIpLabels();
    void update();

private:
    Controller & controller;
    CalibController & calibController;
    map<Position, ShotWidget*> pngWidgets;
    map<Position, QLabel*> ipLabels;

    void initPngWidgets();
};

#endif // ShotsWidget_H
