#include "button.h"

#include <QHBoxLayout>
#include <QFontMetrics>

Button::Button(QWidget *parent)
    : QLabel(parent)
{
    pressed = false;

    setProperty("class", "button");

    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::CursorShape::PointingHandCursor);
}


void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        emit clicked();
    }
}

void Button::mousePressEvent(QMouseEvent *)
{
    pressed = true;
}

void Button::enterEvent(QEnterEvent *)
{
    if (!pressed) {

    }
}

void Button::leaveEvent(QEvent *)
{
    if (!pressed) {

    }
}