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

typedef QPair<QDateTime, double> Timestamp;

class Calendar : public QWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent);

private:
    enum class SpanType : quint8 {
        None,
        Day,
        NoWeekend,
        Week,
        Month,
    };
    QHash<SpanType, qint64> typeDuration;

    const char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    const char* days[8] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    QDir dataFolder;
    QString appdataPath;
    QString dataFolderPath;
    QString calendarFolderPath;

    void loadConfig();
    void loadUI();
    void loadCalendars();
    void resetSpan();
    bool addToConfig(QString name, QVariant value);

    void checkDataFolder();
    QStringList getICSUrls();
    QStringList getICSFilePaths();

    void loadRemoteICS(QString url);
    void loadICS(QString content);
    void loadEvents();

    void updateSizes();

    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    QNetworkAccessManager network;
    QDateTime spanStart;
    QDateTime spanEnd;
    qint64 spanDuration;
    SpanType spanType;
    QTimeZone timezone;

    QHash<QDate, QList<QPair<QDateTime, icalcomponent *>>> components;
    QHash<QDate, QList<QPair<Event *, Timestamp>>> events;
    QList<QPair<Event *, Timestamp>> eventList;

    QVBoxLayout *lay = nullptr;
    QWidget *contentHeader = nullptr;
    QScrollArea *scroll = nullptr;
    QWidget *content = nullptr;
    QWidget *hourContainer = nullptr;
    QWidget *eventField = nullptr;
    EventPage *ep = nullptr;
    QList<QWidget *> dayLabels;

    qint32 columnWidth = 100;
    qint32 rowHeight = 60;
    qint32 offset = 30;
};