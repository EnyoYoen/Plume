#include "bottombar.h"

BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent)
{
    lay = new QHBoxLayout(this);

    homeButton = new BarButton(BarButton::Type::Home, this);
    calendarButton = new BarButton(BarButton::Type::Calendar, this);
    mailButton = new BarButton(BarButton::Type::Mail, this);    

    QObject::connect(homeButton, &BarButton::clicked, [this](){ emit homeButtonPressed(); });
    QObject::connect(calendarButton, &BarButton::clicked, [this](){ emit calendarButtonPressed(); });
    QObject::connect(mailButton, &BarButton::clicked, [this](){ emit mailButtonPressed(); });

    lay->addStretch();
    lay->addWidget(homeButton, 0, Qt::AlignVCenter);
    lay->addWidget(calendarButton, 0, Qt::AlignVCenter);
    lay->addWidget(mailButton, 0, Qt::AlignVCenter);
    lay->addStretch();
    
    lay->setContentsMargins(0, 8, 0, 8);
    lay->setSpacing(20);

    setProperty("class", "bottombar");
    setAttribute(Qt::WA_StyledBackground);
    
    setFixedHeight(80);
}