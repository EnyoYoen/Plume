#pragma once

#include <QWidget>

#include "libical/ical.h"

#include "typedefs.h"

class AddCalendarPage : public QWidget
{
    Q_OBJECT

public:
    AddCalendarPage(QHash<CalendarName, QList<icalcomponent *>> calendarsComponents, QWidget *parent);

signals:
    void closed();
    void urlAdded(QUrl url);
    void fileAdded(QString filename);
};