#include "button.h"

#include <QHBoxLayout>
#include <QFontMetrics>

BarButton::BarButton(BarButton::Type type, QWidget *parent)
    : QLabel(parent)
{
    pressed = false;

    content = new QLabel(this);
    content->setFixedSize(48, 48);

    switch (type)
    {
        case BarButton::Type::Home:
            content->setText("Home");
            break;
        case BarButton::Type::Calendar:
            content->setText("Calendar");
            break;
        case BarButton::Type::Mail:
            content->setText("Mail");
            break;
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(content, 0, Qt::AlignHCenter);

    setProperty("class", "bar-button");

    setCursor(Qt::CursorShape::PointingHandCursor);
    setContentsMargins(8, 8, 8, 8);
    setFixedSize(64, 64);
}


void BarButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        emit clicked();
    }
}

void BarButton::mousePressEvent(QMouseEvent *)
{
    pressed = true;
}

void BarButton::enterEvent(QEnterEvent *)
{
    if (!pressed) {

    }
}

void BarButton::leaveEvent(QEvent *)
{
    if (!pressed) {

    }
}