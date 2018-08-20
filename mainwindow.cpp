#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(lSaveROIButton, SIGNAL (released()), this, SLOT (onLSaveROIButton()));
    connect(rSaveROIButton, SIGNAL (released()), this, SLOT (onRSaveROIButton()));
    connect(lLoadDebugButton, SIGNAL (released()), this, SLOT (onLLoadDebugButton()));
    connect(rLoadDebugButton, SIGNAL (released()), this, SLOT (onRLoadDebugButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::onLSaveROIButton(){

}

MainWindow::onRSaveROIButton(){

}

MainWindow::onLLoadDebugButton(){

}

MainWindow::onRLoadDebugButton(){

}
