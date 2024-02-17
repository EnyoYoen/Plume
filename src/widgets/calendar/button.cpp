#include "button.h"

#include <QHBoxLayout>
#include <QFontMetrics>

HeaderButton::HeaderButton(HeaderButton::Type type, QWidget *parent)
    : QLabel(parent)
{
    pressed = false;

    QString name;
    switch (type)
    {
        case HeaderButton::Type::Left:
            name = "left";
            break;
        case HeaderButton::Type::Right:
            name = "right";
            break;
        case HeaderButton::Type::Today:
            name = "today";
            break;
        case HeaderButton::Type::Settings:
            name = "settings";
            break;
    }
    setText(name);

    setProperty("class", "calendar-header-button calendar-header-button-" + name);

    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::CursorShape::PointingHandCursor);
    setFixedSize(40, 40);
}


void HeaderButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        emit clicked();
    }
}

void HeaderButton::mousePressEvent(QMouseEvent *)
{
    pressed = true;
}

void HeaderButton::enterEvent(QEnterEvent *)
{
    if (!pressed) {

    }
}

void HeaderButton::leaveEvent(QEvent *)
{
    if (!pressed) {

    }
}