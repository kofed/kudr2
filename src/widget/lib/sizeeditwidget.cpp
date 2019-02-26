#include "sizeeditwidget.h"
#include <QHBoxLayout>
#include <QLabel>

SizeEditWidget::SizeEditWidget(QString label):SizeEditWidget(){
   // QHBoxLayout * layout = new QHBoxLayout;
   // layout->addWidget(new QLabel(label));
   // layout->addWidget(this);
   // setLayout(layout);
}

SizeEditWidget::SizeEditWidget():QLineEdit()
{
    setInputMask("99\:99");
    setFixedWidth(30);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

Size SizeEditWidget::getSize(){
    QStringList sl = text().split(":");
    if(sl.size() != 2){
        throw std::runtime_error("заполните размер");
    }
    return Size(sl.at(0).toInt(), sl.at(1).toInt());
}
