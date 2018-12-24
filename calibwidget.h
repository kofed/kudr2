#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "pngwidget.h"
#include "calibcontroller.h"
#include "controller.h"
#include <QLineEdit>

class CalibWidget : public QWidget{
    Q_OBJECT

public:
    explicit CalibWidget( QWidget *parent,  QTableWidget *_table, Controller & _controller );

    virtual    ~CalibWidget(){}

    QTableWidget *table;

    CalibController* calibController = new CalibController();


signals:
    void updateChessBoardImage();
private:
    Controller & controller;

    QString generateFileName(Position pos);


  //  QWidget *parent;

    QHBoxLayout * mainLayout = new QHBoxLayout();
    QPushButton * findCornersButton;
    QPushButton * addButton;
    QPushButton * deleteButton;
    QPushButton * writeButton;
    QPushButton * shotButton;
    QLineEdit* sizeEdit = new QLineEdit();
    QLabel* sizeLabel = new QLabel("Доска");

    void listChessboardImages(const QString & name) const;

    Size parseSize();
private slots:
    void onFindCornersButton();
    void onAddButton();
    void onDeleteButton();
    void onWriteButton();
    void onShotButton();
};

#endif // CALIBWIDGET_H
