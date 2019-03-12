#include "shotwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QMessageBox>

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
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(imageLabel, SIGNAL(rectSelected(QRect)), this, SLOT(onRectSelected(QRect)));
    connect(imageLabel, SIGNAL(pointSelected(QPoint)), this, SLOT(onPointSelected(QPoint)));
    connect(viewer, SIGNAL(scaleFactorChanged(double)), imageLabel, SLOT(setScaleFactor(double)));
}

void ShotWidget::update(){
    QPixmap pixmap = mat2QPixmap(calibController.getImageWithCorners(position));
    viewer->updateImage(pixmap);
    //viewer->resize(pixmap.size());
    viewer->show();
    QApplication::processEvents();
   // viewer->setImage(pixmap);
}

void ShotWidget::init(){
    QPixmap pixmap = mat2QPixmap(calibController.images[position]);
    viewer->setImage(pixmap);
    viewer->show();
    QApplication::processEvents();
}

void ShotWidget::onRectSelected(const QRect & rect){
    calibController.rois[position] = Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

void ShotWidget::onPointSelected(const QPoint & point){
    try {
        float x = (float) point.x();
        float y = (float) point.y();
        Point2f point = Point2f(x, y);

        if(addRadioButton->isChecked()){
            calibController.addCorner(position, point);
        }
        if(deleteRadioButton->isChecked()){
            calibController.deleteCorner(position, point);
        }
        if(selectCenterRadioButton->isChecked()){
            calibController.centers[position] = point;
        }
        addRadioButton->setEnabled(calibController.patternWasFound(position) < 0);
        update();
    } catch (exception & e) {
        QMessageBox::warning(this, e.what(), e.what());
    }

}

QPixmap ShotWidget::mat2QPixmap(const Mat & image){
    if(!image.data){
        return QPixmap();
    }

    return QPixmap::fromImage(QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_RGB888));
}
