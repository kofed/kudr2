#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QString>
#include <QTableWidget>

class CalibWidget {
 //   Q_OBJECT

public:
    CalibWidget( QTableWidget *table);

    ~CalibWidget();

    QTableWidget *table;

private:


    void listChessboardImages(const QString & name) const;
};

#endif // CALIBWIDGET_H
