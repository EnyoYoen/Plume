#include "addcalendarpage.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLineEdit>

#include "backbutton.h"

QWidget *buildCalendarWidget(CalendarName name, qsizetype eventCount, QWidget *parent)
{
    QWidget *calendar = new QWidget(parent);
    calendar->setProperty("class", "calendar-add-calendar-page-calendar");

    QHBoxLayout *calendarLay = new QHBoxLayout(calendar);
    calendarLay->setSpacing(0);
    calendarLay->setContentsMargins(0, 0, 0, 0);

    QLabel *calendarName = new QLabel(name.first.isNull() ? name.second.fileName() : name.first, parent);
    calendar->setProperty("class", "calendar-add-calendar-page-calendar-name");
    calendarLay->addWidget(calendarName);

    calendarLay->addStretch();
    
    QLabel *events = new QLabel(QString::number(eventCount) + " incoming " + (eventCount > 1 ? "events" : "event"));
    calendar->setProperty("class", "calendar-add-calendar-page-calendar-events");
    calendarLay->addWidget(events);

    return calendar;
}

AddCalendarPage::AddCalendarPage(QHash<CalendarName, QList<icalcomponent *>> calendarsComponents, QWidget *parent)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);


    QWidget *header = new QWidget(this);
    QHBoxLayout *headerLay = new QHBoxLayout(header);
    BackButton *backButton = new BackButton(header);
    QObject::connect(backButton, &BackButton::clicked, [this](){ emit closed(); });
    QLabel *title = new QLabel("Calendars", header);

    title->setProperty("class", "calendar-add-calendar-page-title");
    header->setProperty("class", "calendar-add-calendar-page-header");
    headerLay->addWidget(backButton);
    headerLay->addWidget(title);
    headerLay->setContentsMargins(10, 10, 10, 10);
    headerLay->setSpacing(20);
    lay->addWidget(header);
        

    QScrollArea *scroll = new QScrollArea(this);
    QWidget *scrollContent = new QWidget(scroll);
    QVBoxLayout *scrollLay = new QVBoxLayout(scrollContent);
    scrollLay->setContentsMargins(5, 5, 5, 5);
    scrollLay->setSpacing(0);
    
    QLabel *sync = new QLabel("Synchronised calendars", scrollContent);
    sync->setProperty("class", "calendar-add-calendar-page-subtitle");
    QWidget *line1 = new QWidget(scrollContent);
    line1->setFixedSize(170, 10);
    line1->setProperty("class", "calendar-add-calendar-page-line");
    line1->setAttribute(Qt::WA_StyledBackground, true);
    scrollLay->addWidget(sync);
    scrollLay->addWidget(line1);

    for (CalendarName name : calendarsComponents.keys()) {
        QWidget *calendar = buildCalendarWidget(name, calendarsComponents[name].size(), scrollContent);
        scrollLay->addWidget(calendar);
    }


    QLabel *add = new QLabel("Add calendars", scrollContent);
    add->setProperty("class", "calendar-add-calendar-page-subtitle");
    QWidget *line2 = new QWidget(scrollContent);
    line2->setFixedSize(170, 10);
    line2->setProperty("class", "calendar-add-calendar-page-line");
    line2->setAttribute(Qt::WA_StyledBackground, true);
    scrollLay->addWidget(add);
    scrollLay->addWidget(line2);

    QLineEdit *urlInput = new QLineEdit(scrollContent);
    urlInput->setPlaceholderText("Enter an URL here");
    urlInput->setProperty("class", "calendar-add-calendar-page-url");
    QObject::connect(urlInput, &QLineEdit::returnPressed, [this, urlInput]() {
        QUrl url(urlInput->text().trimmed());
        if (url.isValid()) {
            urlInput->clear();
            emit urlAdded(url);
        }
    });
    scrollLay->addWidget(urlInput);

    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setProperty("class", "calendar-add-calendar-page-scroll");
    lay->addWidget(scroll);


    setProperty("class", "calendar-add-calendar-page");
    setAttribute(Qt::WA_StyledBackground);
}