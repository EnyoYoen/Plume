#include "barbutton.h"

#include <QHBoxLayout>
#include <QFontMetrics>

BarButton::BarButton(BarButton::Type type, QWidget *parent)
    : Button(parent)
{
    content = new QLabel(this);
    content->setFixedSize(48, 48);

    switch (type)
    {
        case BarButton::Type::Home:
            content->setPixmap(QPixmap(":/home.png").scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            break;
        case BarButton::Type::Calendar:
            content->setPixmap(QPixmap(":/calendar.png").scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            break;
        case BarButton::Type::Mail:
            content->setPixmap(QPixmap(":/email.png").scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            break;
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(content, 0, Qt::AlignHCenter);

    setProperty("class", property("class").toString() + " bar-button");

    setContentsMargins(8, 8, 8, 8);
    setFixedSize(64, 64);
}