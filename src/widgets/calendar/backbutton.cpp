#include "backbutton.h"

BackButton::BackButton(QWidget *parent)
    : Button(parent)
{
    setPixmap(QPixmap(":/left.png").scaledToWidth(25, Qt::TransformationMode::SmoothTransformation));
    setProperty("class", property("class").toString() + " calendar-event-page-button");
    setFixedSize(40, 40);
}