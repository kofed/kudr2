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

    CalibController & calibController;
public:
    void setImage(const QPixmap & pixmap);

    ShotWidget(Position _position, CalibController & _calibController);

public slots:
    void onRectSelected(const QRect & rect);

    void onCenterSelected(const QPoint & center);
};

#endif // SHOTWIDGET_H
