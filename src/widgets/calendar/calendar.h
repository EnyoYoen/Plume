#pragma once

#include <QShortcut>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

#include <QNetworkAccessManager>
#include <QTimeZone>

#include "libical/ical.h"

#include "typedefs.h"
#include "config.h"
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
    // TODO: Replace with the Qt translation system
    const char* months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    const char* days[8] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    void loadUI();
    void loadCalendars();
    void resetSpan();
    void resetCalendars();

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

    Config config;

    qint32 columnWidth = 100;
    qint32 rowHeight = 60;
    qint32 offset = 30;
};