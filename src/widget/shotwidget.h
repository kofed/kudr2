#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H

#include "lib/imageviewer.h"
#include "lib/sizeeditwidget.h"
#include <QRadioButton>
#include "controller.h"
#include "calibcontroller.h"
#include "lib/selectiveImageLabel.h"

class ShotWidget : public QWidget
{
    Q_OBJECT

private:
    const Position position;

    ImageViewer * viewer;
    SelectiveImageLabel * imageLabel;

    SizeEditWidget * positionEdit = new SizeEditWidget;

    QRadioButton * deleteRadioButton = new QRadioButton("Удалить угол");
    QRadioButton * addRadioButton = new QRadioButton("Добавить угол");
    QRadioButton * selectCenterRadioButton = new QRadioButton("Выделить центр");

    SizeEditWidget * addedCornerIndex = new SizeEditWidget("Позиция добавленного угла");

    CalibController & calibController;
public:
    void update();

    ShotWidget(Position _position, CalibController & _calibController);

    QPixmap mat2QPixmap(const Mat & image);

public slots:
    void onRectSelected(const QRect & rect);

    void onPointSelected(const QPoint & point);
};

#endif // SHOTWIDGET_H
