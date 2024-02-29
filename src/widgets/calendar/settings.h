#pragma once

#include <QWidget>
#include <QLabel>

#include "typedefs.h"

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(QHash<CalendarName, bool> calendarsEnabled, QWidget *parent);

signals:
    void closed();
    void calendarTypeClicked(quint8 index);
    void calendarsClicked();
    void calendarClicked(CalendarName name, bool enabled);

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    QWidget *innerContent = nullptr;

    QHash<CalendarName, bool> calendarsEnabled;
};