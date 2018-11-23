#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QPushButton>

class CalibWidget {
    enum{
        COLUMN_FILE_NAME
    };

public:
    CalibWidget(const QTableWidget *table,  const QPushButton * _saveShotButton);

    ~CalibWidget();

    QTableWidget *table;

private:

    const QPushButton * saveShotButton;

    void listChessboardImages(const QString & name) const;
};

#endif // CALIBWIDGET_H
