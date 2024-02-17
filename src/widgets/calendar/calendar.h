#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QDir>
#include <QTimeZone>

#include "libical/ical.h"

#include "event.h"
#include "eventpage.h"

class Calendar : public QWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent);

private:
    enum class SpanType : quint8 {
        NoWeekend,
        Week,
        Month,
    };

    const char* days[8] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    QDir dataFolder;
    QString appdataPath;
    QString dataFolderPath;
    QString calendarFolderPath;

    void loadUI();
    void loadCalendars();

    void checkDataFolder();
    QStringList getICSUrls();
    QStringList getICSFilePaths();

    void loadRemoteICS(QString url);
    void loadICS(QString content);

    QNetworkAccessManager network;
    QDateTime spanStart;
    QDateTime spanEnd;
    qint64 spanDuration;
    SpanType spanType;

    QTimeZone timezone;
    QHash<QDate, QList<QPair<QDateTime, icalcomponent *>>> components;
    QList<Event *> events;

    QVBoxLayout *lay = nullptr;
    QWidget *header = nullptr;
    QScrollArea *scroll = nullptr;
    QWidget *eventField = nullptr;
    EventPage *ep = nullptr;
    qint32 columnWidth = 100;
    qint32 rowHeight = 60;
    qint32 offset = 30;
};