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
    calendarLay->setContentsMargins(10, 0, 10, 0);

    QLabel *calendarName = new QLabel(name.first.isNull() ? name.second.fileName() : name.first, parent);
    calendarName->setProperty("class", "calendar-add-calendar-page-calendar-name");
    calendarLay->addWidget(calendarName);

    calendarLay->addStretch();
    
    QLabel *events = new QLabel(QString::number(eventCount) + " incoming " + (eventCount > 1 ? "events" : "event"));
    events->setProperty("class", "calendar-add-calendar-page-calendar-events");
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

    title->setProperty("class", "calendar-page-title");
    header->setProperty("class", "calendar-page-header");
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
    sync->setProperty("class", "calendar-page-subtitle");
    QWidget *line1 = new QWidget(scrollContent);
    line1->setFixedSize(170, 10);
    line1->setProperty("class", "calendar-page-line");
    line1->setAttribute(Qt::WA_StyledBackground, true);
    scrollLay->addSpacing(10);
    scrollLay->addWidget(sync);
    scrollLay->addSpacing(4);
    scrollLay->addWidget(line1);
    scrollLay->addSpacing(8);

    for (CalendarName name : calendarsComponents.keys()) {
        QWidget *calendar = buildCalendarWidget(name, calendarsComponents[name].size(), scrollContent);
        scrollLay->addWidget(calendar);
    }

    scrollLay->addSpacing(50);


    QLabel *add = new QLabel("Add calendars", scrollContent);
    add->setProperty("class", "calendar-page-subtitle");
    QWidget *line2 = new QWidget(scrollContent);
    line2->setFixedSize(170, 10);
    line2->setProperty("class", "calendar-page-line");
    line2->setAttribute(Qt::WA_StyledBackground, true);
    scrollLay->addWidget(add);
    scrollLay->addSpacing(4);
    scrollLay->addWidget(line2);
    scrollLay->addSpacing(8);

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
    scrollLay->addStretch();

    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setProperty("class", "calendar-page-scroll");
    lay->addWidget(scroll);


    setProperty("class", "calendar-page");
    setAttribute(Qt::WA_StyledBackground);
}