#pragma once

#include <QShortcut>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

#include <QNetworkAccessManager>
#include <QDir>
#include <QTimeZone>

#include "libical/ical.h"

#include "typedefs.h"
#include "headerbutton.h"
#include "event.h"
#include "eventpage.h"
#include "settings.h"
#include "addcalendarpage.h"

class Calendar : public QWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent);

private slots:
    void openSettings();

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
    void resetCalendars();
    bool addToConfig(QString name, QVariant value);
    bool addToConfig(QString name, QHash<CalendarName, bool> value);

    void checkDataFolder();
    QStringList getICSUrls();
    QStringList getICSFilePaths();

    void loadRemoteICS(QString url);
    void loadICS(QString content, QString name, QUrl url);
    void loadEvents();

    void updateArrows();
    void updateSizes();

    void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    QNetworkAccessManager network;
    QDateTime spanStart;
    QDateTime spanEnd;
    qint64 spanDuration;
    SpanType spanType;
    QTimeZone timezone;
    quint8 month;
    qint64 year;
    bool reset = false;

    QHash<QDate, QHash<icalcomponent *, QDateTime>> components;
    QHash<CalendarName, QList<icalcomponent *>> calendarsComponents;
    QHash<icalcomponent *, bool> hiddenComponents;
    QHash<icalcomponent *, QDate> componentsIndex;
    QHash<QDate, QList<QPair<Event *, Timestamp>>> events;
    QList<QPair<Event *, Timestamp>> eventList;
    QHash<CalendarName, bool> disabledCalendars;

    QVBoxLayout *lay = nullptr;
    QWidget *contentHeader = nullptr;
    QHBoxLayout *contentHeaderLay = nullptr;
    QWidget *header = nullptr;
    QLabel *headerDate = nullptr;
    HeaderButton *left = nullptr;
    HeaderButton *right = nullptr;
    HeaderButton *today = nullptr;
    HeaderButton *settings = nullptr;
    QWidget *arrowSpacer1 = nullptr;
    QWidget *arrowSpacer2 = nullptr;
    QScrollArea *scroll = nullptr;
    QWidget *content = nullptr;
    QWidget *hourContainer = nullptr;
    QWidget *eventField = nullptr;
    EventPage *ep = nullptr; // TODO : close these pages when switching to another widgets, to the settings or to an other span 
    AddCalendarPage *addCalendarPage = nullptr;
    Settings *settingsPopUp = nullptr;
    QList<QWidget *> dayLabels;

    QShortcut *leftShortcut = nullptr;
    QShortcut *rightShortcut = nullptr;
    QShortcut *todayShortcut = nullptr;
    QShortcut *settingsShortcut = nullptr;

    QTimer *refreshTimer = nullptr;

    qint32 columnWidth = 100;
    qint32 rowHeight = 60;
    qint32 offset = 30;
};