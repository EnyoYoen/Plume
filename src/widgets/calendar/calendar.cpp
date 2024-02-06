#include "calendar.h"

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
{
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    QWidget *header = new QWidget(this);
    header->setFixedHeight(30);
    QHBoxLayout *headerLay = new QHBoxLayout(header); 

    for (quint8 i = 0 ; i < 8 ; i++) {
        QLabel *day = new QLabel(days[i], header);
        if (i == 0) {
            day->setFixedWidth(30);
        } else {
            day->setProperty("class", "calendar-day");
        }
        headerLay->addWidget(day);
    }

    headerLay->setContentsMargins(0, 0, 0, 0);

    scroll = new QScrollArea(this);
    scroll->setProperty("class", "calendar-scroll");
    
    QWidget *content = new QWidget(scroll);
    QHBoxLayout *contentLay = new QHBoxLayout(content);

    QWidget *hours = new QWidget(content);
    hours->setFixedWidth(30);
    QVBoxLayout *hoursLay = new QVBoxLayout(hours);

    for (quint8 i = 1 ; i < 25 ; i++) {
        QLabel *hour = new QLabel(QString::number(i) + "h", hours);
        hour->setProperty("class", "calendar-hour");
        hour->setFixedHeight(60);
        hoursLay->addWidget(hour, 0, Qt::AlignHCenter);
    }

    hoursLay->setContentsMargins(0, 0, 0, 0);

    contentLay->addWidget(hours);
    contentLay->addStretch();
    contentLay->setContentsMargins(0, 0, 0, 0);

    scroll->setWidget(content);
    scroll->setContentsMargins(0, 0, 0, 0);
    scroll->setWidgetResizable(true);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lay->addWidget(header);
    lay->addWidget(scroll);





    setProperty("class", "calendar");
    setAttribute(Qt::WA_StyledBackground);
}