#pragma once

#include <QWidget>
#include <QLabel>

#include "typedefs.h"

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(QList<CalendarName> calendarsNames, QWidget *parent);

signals:
    void closed();
    void calendarTypeClicked(quint8 index);
    void calendarsClicked();
    void calendarClicked(CalendarName name, bool enabled);

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    QWidget *innerContent = nullptr;

    QHash<CalendarName, bool> enable;
};