#include "home.h"

Home::Home(QWidget *parent)
    : QWidget(parent)
{
    lay = new QHBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    setProperty("class", "home");
    setAttribute(Qt::WA_StyledBackground);
}