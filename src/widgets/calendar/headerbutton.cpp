#include "headerbutton.h"

HeaderButton::HeaderButton(HeaderButton::Type type, QWidget *parent)
    : Button(parent)
{
    QString name;
    QString icon;
    switch (type)
    {
        case HeaderButton::Type::Left:
            name = "left";
            icon = ":/left-arrow.png";
            break;
        case HeaderButton::Type::Right:
            name = "right";
            icon = ":/right-arrow.png";
            break;
        case HeaderButton::Type::Today:
            name = "today";
            icon = ":/calendar.png";
            break;
        case HeaderButton::Type::Settings:
            name = "settings";
            icon = ":/three-dots.png";
            break;
    }
    setText(name);
    setPixmap(QPixmap(icon).scaledToWidth(40, Qt::TransformationMode::SmoothTransformation));

    setProperty("class", property("class").toString() + " calendar-header-button");
    setFixedSize(40, 40);
}