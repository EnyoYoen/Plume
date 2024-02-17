#include "event.h"

Event::Event(QDateTime datetime, icalcomponent *comp, QWidget *parent)
    : QWidget(parent)
{
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(5, 5, 5, 5);
    lay->setSpacing(0);

    QLabel *title = new QLabel(this);
    QLabel *desc = new QLabel(this);

    const char *description = icalcomponent_get_description(comp);
    if (QString(icalcomponent_get_uid(comp)).startsWith("ADE")) {
        QString qdesc(description);
        QStringList splits = qdesc.split('\n');
        splits.removeAll("");

        if (splits.length() > 0) {
            QStringList intro = splits[0].split(']');
            if (intro.length() > 1) {
                desc->setText(intro[1].trimmed());
                desc->setProperty("class", "calendar-event-desc-ade");

                title->setText(intro[0].split(':').last());
                title->setProperty("class", "calendar-event-title-ade");

                setProperty("class", "calendar-event-" + intro[0].split(':').last().toLower());
            }
        }
    } else {
        title->setText(icalcomponent_get_summary(comp));
        desc->setText(description);
    }

    QLabel *location = new QLabel(icalcomponent_get_location(comp), this); 

    title->setWordWrap(true);
    desc->setWordWrap(true);
    location->setWordWrap(true);

    lay->addWidget(title);
    lay->addWidget(location);
    lay->addWidget(desc);
    //lay->addStretch();

    location->setProperty("class", "calendar-event-location");
    title->setProperty("class", title->property("class").toString() + " calendar-event-title");
    desc->setProperty("class", title->property("class").toString() + " calendar-event-desc");

    setProperty("class", property("class").toString() + " calendar-event");

    setAttribute(Qt::WA_StyledBackground);
}

void Event::reduce(qint8 count)
{
    reduceFactor += count;
}


void Event::showEvent(QShowEvent *)
{
    if (!reduced) {
        reduced = true;
        int width = this->width();
        this->setFixedWidth(this->width() / reduceFactor);
        this->move(this->x() + width / reduceFactor * order, this->y());
    }
}

void Event::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        emit clicked();
    }
}

void Event::mousePressEvent(QMouseEvent *)
{
    pressed = true;
}

void Event::enterEvent(QEnterEvent *)
{
    if (!pressed) {

    }
}

void Event::leaveEvent(QEvent *)
{
    if (!pressed) {

    }
}