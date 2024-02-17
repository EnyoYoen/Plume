#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDateTime>

#include "libical/ical.h"

class EventPage : public QWidget
{
    Q_OBJECT

public:
    EventPage(icalcomponent *comp, QDateTime datetime, qint32 duration, QWidget *parent);

signals:
    void closed();

private:
    QVBoxLayout *lay = nullptr;
};