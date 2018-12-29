#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H
#include <QWidget>
#include <QLabel>
#include <map>
#include "pngwidget.h"
#include "controller.h"
#include "position.h"

class ShotWidget : public QWidget
{
    Q_OBJECT

private:
    Controller & controller;
    std::map<Position, QLabel> pngLabels;
    std::map<Position, PngWidget> pngWidgets;

    QLabel* lIpLabel = new QLabel();

    QLabel* rIpLabel = new QLabel();
public:
    explicit ShotWidget(Controller & _controller);

    virtual ~ShotWidget(){}

signals:
    void imageUpdated();
public slots:
    void ipChanged();

};

#endif // SHOTWIDGET_H
