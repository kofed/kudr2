#include "sizeeditwidget.h"
#include <QHBoxLayout>
#include <QLabel>

SizeEditWidget::SizeEditWidget(QString label):SizeEditWidget(){
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(new QLabel(label));


    layout->addWidget(edit);
    setLayout(layout);
}

SizeEditWidget::SizeEditWidget():QWidget()
{
    edit = new QLineEdit;
    edit->setInputMask("99\:99");
    edit->setFixedWidth(50);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

Size SizeEditWidget::getSize(){
    QStringList sl = edit->text().split(":");
    if(sl.size() != 2){
        throw std::runtime_error("заполните размер");
    }
    return Size(sl.at(0).toInt(), sl.at(1).toInt());
}

QString SizeEditWidget::text(){
    return edit->text();
}

void SizeEditWidget::clear(){
    edit->clear();
}
