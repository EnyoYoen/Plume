#include "enablebutton.h"

EnableButton::EnableButton(bool enable, QWidget *parent)
    : QLabel(parent)
{
    setEnable(enable);

    setFixedSize(20, 20);
}

void EnableButton::setEnable(bool enable)
{
    enabled = enable;

    setProperty("class", "calendar-enable-button calendar-enable-button-" + QString(enabled ? "enabled" : "disabled"));
    
    // TODO : do that by applying the global stylesheet (or create a new little stylesheet for this button)
    // TODO : add an icon when enabled instead of putting plain orange
    if (enable) {
        setStyleSheet(
            "background-color: #f69f1c;"
            "border-radius: 4px;"
            "border: 2px solid #f69f1c;");
    } else {
        setStyleSheet(
            "background-color: none;"
            "border-radius: 4px;"
            "border: 2px solid #cccccc;");
    }
}