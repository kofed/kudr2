#include "shotwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

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

    hLayout->addWidget(selectCenterRadioButton);
    hLayout->addWidget(deleteRadioButton);
    hLayout->addWidget(addRadioButton);
    hLayout->addWidget(addedCornerIndex);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    addedCornerIndex->setEnabled(false);

    connect(imageLabel, SIGNAL(rectSelected(QRect)), this, SLOT(onRectSelected(QRect)));
    connect(imageLabel, SIGNAL(pointSelected(QPoint)), this, SLOT(onPointSelected(QPoint)));
    connect(addRadioButton, SIGNAL(toggled(bool)), addedCornerIndex, SLOT(setEnabled(bool)));
}

void ShotWidget::update(){
    QPixmap pixmap = mat2QPixmap(calibController.getImageWithCorners(position));
    viewer->setImage(pixmap);
    viewer->resize(pixmap.size());
    viewer->show();
    QApplication::processEvents();
    viewer->setImage(pixmap);
}

void ShotWidget::onRectSelected(const QRect & rect){
    calibController.rois[position] = Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

void ShotWidget::onPointSelected(const QPoint & point){
    Point2i point2i = Point2i(point.x(), point.y());

    if(addRadioButton->isChecked()){
        calibController.addCorner(position, point2i, addedCornerIndex->getSize());
    }
    if(deleteRadioButton->isChecked()){
        calibController.deleteCorner(position, point2i);
    }
    if(selectCenterRadioButton->isChecked()){
        calibController.centers[position] = point2i;
    }
    update();
}


QPixmap ShotWidget::mat2QPixmap(const Mat & image){
    if(!image.data){
        return QPixmap();
    }

    return QPixmap::fromImage(QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_RGB888));
}
