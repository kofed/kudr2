#include "shotwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "lib/pngwidget.h"

ShotWidget::ShotWidget(CalibController & _calibController)
    :calibController(_calibController), QWidget()
{
    viewer = new ImageViewer(new PngWidget());

    QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->addWidget(viewer);

    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout->addWidget(addRadioButton);
    hLayout->addWidget(deleteRadioButton);

    vLayout->addLayout(hLayout);
    setLayout(vLayout);

}

void ShotWidget::setImage(const QPixmap &pixmap){
    viewer->setImage(pixmap);
}
