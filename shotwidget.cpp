#include "shotwidget.h"

ShotWidget::ShotWidget()
{
    QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout.add(viewer);

    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout.addWidget(addRadioButton);
    hLayout.addWidget(deleteRadioButton);

}
