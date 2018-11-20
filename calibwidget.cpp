#include "calibwidget.h"
#include "ui_calibwidget.h"

CalibWidget::CalibWidget(const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibWidget){
    ui->setupUi(this);
    setName(name);
}

CalibWidget::~CalibWidget(){
    delete ui;
}
