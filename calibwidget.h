#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class CalibWidget;
}

class CalibWidget : public QWidget{
    Q_OBJECT

public:
    explicit CalibWidget(const QString& name, QWidget *parent = 0);

    ~CalibWidget();

private:
    Ui::CalibWidget *ui;
}

#endif // CALIBWIDGET_H
