#pragma once

#include <QString>
#include <QHash>
#include <QDir>

#include "typedefs.h"

class CalendarConfig {

public:
    CalendarConfig() = default;

    void load();
    bool add(QString name, QVariant value);
    bool add(QString name, QHash<CalendarName, bool> value);

    void checkDataFolder();
    QFile getICSFile(QString name);
    QStringList getICSUrls();
    QStringList getICSFilePaths();

    QHash<CalendarName, bool> disabledCalendars;
    QHash<SpanType, qint64> typeDuration;
    SpanType spanType;
    qint64 spanDuration;

private:
    QDir dataFolder;

    QString appdataPath;
    QString dataFolderPath;
    QString calendarFolderPath;
};
