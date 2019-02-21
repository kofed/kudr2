#include "sizeeditwidget.h"

SizeEditWidget::SizeEditWidget():QLineEdit()
{
    setInputMask("99\:99");
    setFixedWidth(30);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

Size SizeEditWidget::getSize(){
    QStringList sl = text().split(":");
    if(sl.size() != 2){
        throw std::runtime_error("заполните размер шахматной доски");
    }
    return Size(sl.at(0).toInt(), sl.at(1).toInt());
}
