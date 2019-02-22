#include "shotwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ShotWidget::ShotWidget(
        Position _position,
        CalibController & _calibController)
    :QWidget(), position(_position), calibController(_calibController)
{
    imageLabel = new SelectiveImageLabel(this);
    viewer = new ImageViewer(imageLabel);

    QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->addWidget(viewer);

    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout->addWidget(addRadioButton);
    hLayout->addWidget(deleteRadioButton);

    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(imageLabel, SIGNAL(rectSelected(QRect)), this, SLOT(onRectSelected(QRect)));
    connect(imageLabel, SIGNAL(centerSelected(Point2i)), this, SLOT(onCenterSelected(Point2i)));
}

void ShotWidget::setImage(const QPixmap &pixmap){
    viewer->setImage(pixmap);
}

void ShotWidget::onRectSelected(const QRect & rect){
    calibController.rois[position] = Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

void ShotWidget::onCenterSelected(Point2i & center){
    calibController.centers[position] = center;
}
