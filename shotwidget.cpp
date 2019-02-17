#include "shotwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ShotWidget::ShotWidget(CalibController & _calibController)
    :calibController(_calibController), QWidget()
{
    QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->addWidget(viewer);

    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout->addWidget(addRadioButton);
    hLayout->addWidget(deleteRadioButton);

}

void ShotWidget::setImage(const QPixmap &pixmap){
    viewer->setImage(pixmap);
}
