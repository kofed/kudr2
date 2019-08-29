#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "lib/selectiveImageLabel.h"
#include "calibcontroller.h"
#include "controller.h"
#include <QLineEdit>
#include "lib/sizeeditwidget.h"

class CalibWidget : public QWidget{
    Q_OBJECT

public:
    explicit CalibWidget(Controller & _controller, QWidget *parent = 0 );

    virtual    ~CalibWidget(){}

    QTableWidget *table = new QTableWidget;
    QLineEdit* hEdit = new QLineEdit;

    CalibController* calibController;

    void sendYML();

public slots:
    void setEnabled();

private:
    Controller & controller;

    QString generateFileName(Position pos);

    QPushButton * findCornersButton;
    QPushButton * addButton;
    QPushButton * deleteButton;
    QPushButton * writeButton;

    map<Position, SizeEditWidget*> sizeEdits;
    QLabel* sizeLabel = new QLabel("Доска");

    QLineEdit* cellSizeEdit = new QLineEdit;

    void listChessboardImages(const QString & name) const;
private slots:
    void onFindCornersButton();
    void onAddButton();
    void onDeleteButton();
    void onWriteButton();

    void onLeftSizeRedactionFinished();
    void onRightSizeRedactionFinished();
signals:
    void updateChessBoardImage();
};

#endif // CALIBWIDGET_H
