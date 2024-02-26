#include "enablebutton.h"

EnableButton::EnableButton(bool enable, QWidget *parent)
    : QLabel(parent)
{
    setEnable(enable);

    setFixedSize(14, 14);
}

void EnableButton::setEnable(bool enable)
{
    enabled = enable;

    setProperty("class", "calendar-enable-button calendar-enable-button-" + QString(enabled ? "enabled" : "disabled"));
}