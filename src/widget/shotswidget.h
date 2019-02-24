#ifndef ShotsWidget_H
#define ShotsWidget_H
#include <QWidget>
#include <QLabel>
#include <map>
#include "lib/selectiveImageLabel.h"
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

signals:
    void imageUpdated();
public:
    ShotsWidget(Controller & _controller, CalibController & _calibController);

    virtual ~ShotsWidget(){}



public slots:
    void updateIpLabels();
    void update();

private:
    Controller & controller;
    CalibController & calibController;
    map<Position, ShotWidget*> pngWidgets;
    map<Position, QLabel*> ipLabels;

    QPixmap mat2QPixmap(const Mat & image);
};

#endif // ShotsWidget_H
