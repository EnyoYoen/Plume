#include "calendar.h"

#include "button.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QStandardPaths>
#include <QFileInfo>

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
{
    loadConfig();
    loadUI();
    loadCalendars();
}

void Calendar::loadConfig()
{
    typeDuration.insert(SpanType::None,      0);
    typeDuration.insert(SpanType::Day,       1);
    typeDuration.insert(SpanType::NoWeekend, 5);
    typeDuration.insert(SpanType::Week,      7);
    typeDuration.insert(SpanType::Month,     30);

    checkDataFolder();

    spanType = SpanType::NoWeekend;

    QFile configFile(calendarFolderPath + "config.txt");
    if (configFile.open(QFile::ReadOnly)) {
        QString config = configFile.readAll();
        QStringList configLines = config.split('\n');
        for (QString line : configLines) {
            if (line.startsWith("spanType")) {
                QStringList splits = line.split(':');
                if (splits.size() == 3) {
                    bool ok = true;
                    int result = splits[2].toInt(&ok);
                    if (ok && (spanType < SpanType::Month && spanType > SpanType::None)) {
                        spanType = (SpanType)result;
                    }
                }
            }
        }
    }

    spanDuration = typeDuration[spanType];
}

void Calendar::loadUI()
{
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    QWidget *header = new QWidget(this);
    header->setProperty("class", "calendar-header");
    header->setFixedHeight(60);
    QHBoxLayout *headerLay = new QHBoxLayout(header);
    headerLay->setContentsMargins(20, 10, 20, 10);
    headerLay->setSpacing(0);

    QDate currentDate = QDate::currentDate();
    QLabel *headerDate = new QLabel(months[currentDate.month()] + QString(" ") + QString::number(currentDate.year()), header);
    headerDate->setProperty("class", "calendar-header-date");

    HeaderButton *left = new HeaderButton(HeaderButton::Type::Left, header);
    HeaderButton *right = new HeaderButton(HeaderButton::Type::Right, header);
    HeaderButton *today = new HeaderButton(HeaderButton::Type::Today, header);
    HeaderButton *settings = new HeaderButton(HeaderButton::Type::Settings, header);

    QObject::connect(left, &HeaderButton::clicked, [this]() {
        spanStart = spanStart.addDays(-7);
        spanEnd = spanEnd.addDays(-7);
        loadEvents();
    });
    QObject::connect(right, &HeaderButton::clicked, [this]() {
        spanStart = spanStart.addDays(7);
        spanEnd = spanEnd.addDays(7);
        loadEvents();
    });
    QObject::connect(today, &HeaderButton::clicked, [this]() {
        resetSpan();
        loadEvents();
    });
    
    headerLay->addWidget(headerDate, 0, Qt::AlignVCenter);
    headerLay->addStretch();
    headerLay->addWidget(left, 0, Qt::AlignVCenter);
    headerLay->addSpacing(20);
    headerLay->addWidget(right, 0, Qt::AlignVCenter);
    headerLay->addStretch();
    headerLay->addWidget(today, 0, Qt::AlignVCenter);
    headerLay->addSpacing(20);
    headerLay->addWidget(settings, 0, Qt::AlignVCenter);


    QWidget *body = new QWidget(this);
    QVBoxLayout *bodyLay = new QVBoxLayout(body);
    bodyLay->setContentsMargins(0, 0, 0, 0);
    bodyLay->setSpacing(0);

    contentHeader = new QWidget(body);
    contentHeader->setFixedHeight(30);
    QHBoxLayout *contentHeaderLay = new QHBoxLayout(contentHeader); 

    for (quint8 i = 0 ; i < (spanDuration > 7 ? 8 : spanDuration + 1) ; i++) {
        QLabel *day = new QLabel(days[i], contentHeader);
        if (i == 0) {
            day->setFixedWidth(offset);
        } else {
            day->setFixedWidth(columnWidth);
            day->setAlignment(Qt::AlignCenter);
            day->setProperty("class", "calendar-day");
        }
        dayLabels.append(day);
        contentHeaderLay->addWidget(day, 0, Qt::AlignVCenter);
    }

    contentHeaderLay->addStretch();
    contentHeaderLay->setContentsMargins(0, 0, 0, 0);
    contentHeaderLay->setSpacing(0);

    scroll = new QScrollArea(this);
    scroll->setProperty("class", "calendar-scroll");
    scroll->setAttribute(Qt::WA_StyledBackground);
    
    content = new QWidget(scroll);
    QHBoxLayout *contentLay = new QHBoxLayout(content);

    QWidget *hours = new QWidget(content);
    hours->setFixedWidth(offset);
    QVBoxLayout *hoursLay = new QVBoxLayout(hours);

    for (quint8 i = 0 ; i < 25 ; i++) {
        hourContainer = new QWidget(hours);
        QVBoxLayout *hourLay = new QVBoxLayout(hourContainer);
        hourLay->setContentsMargins(0, 0, 0, 0);
        hourLay->setSpacing(0);

        QLabel *hour = new QLabel(QString::number(i) + "h", hourContainer);
        hour->setProperty("class", "calendar-hour");
        hourContainer->setFixedHeight(rowHeight);
        
        hourLay->addWidget(hour, 0, Qt::AlignTop | Qt::AlignHCenter);
        hourLay->addStretch();
        hoursLay->addWidget(hourContainer, 0, Qt::AlignHCenter);
    }

    hoursLay->setContentsMargins(0, 0, 0, 0);
    hoursLay->setSpacing(0);

    eventField = new QWidget(content);
    eventField->setProperty("class", "calendar-event-field");

    contentLay->addWidget(hours);
    contentLay->addWidget(eventField);
    contentLay->setContentsMargins(0, 0, 0, 0);
    contentLay->setSpacing(0);

    scroll->setWidget(content);
    scroll->setContentsMargins(0, 0, 0, 0);
    scroll->setWidgetResizable(true);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    bodyLay->addWidget(contentHeader);
    bodyLay->addWidget(scroll);

    lay->addWidget(header);
    lay->addWidget(body);

    setProperty("class", "calendar");
    setAttribute(Qt::WA_StyledBackground);
}

void Calendar::loadCalendars()
{
    resetSpan();

    checkDataFolder();

    for (QString url : getICSUrls())
        loadRemoteICS(url);

    for (QString filename : getICSFilePaths()) {
        QFile file(calendarFolderPath + "ics/" + filename);
        file.open(QFile::ReadOnly);
        loadICS(file.readAll());
    }
}

void Calendar::resetSpan()
{
    QDateTime datetime = QDateTime::currentDateTime();
    timezone = datetime.timeZone();
    spanStart = QDateTime(datetime.date(), QTime(0, 0));
    switch (spanType)
    {
        case SpanType::Month:
            spanStart = spanStart.addDays(1 - spanStart.date().day());
            break;

        default: // NoWeekend or Week
            spanStart = spanStart.addDays(1 - spanStart.date().dayOfWeek());
    }
    spanEnd = QDateTime(spanStart.date().addDays(spanDuration), QTime(0, 0));
}

bool Calendar::addToConfig(QString name, QVariant value)
{
    checkDataFolder();

    bool found = false;
    QStringList configLines;
    QString configLine = name + ":" + value.toString() + '\n';
    QFile configFile(calendarFolderPath + "config.txt");
    if (configFile.open(QFile::ReadOnly)) {
        QString config = configFile.readAll();
        configLines = config.split('\n');
        for (quint64 i = 0 ; i < configLines.size() ; i++) {
            if (configLines[i].startsWith(name)) {
                configLines[i] = configLine;
            }
        }
    }
    if (!found) {
        configLines.append(configLine);
    }
    configFile.close();

    bool ok = true;
    if (configFile.open(QFile::WriteOnly)) {
        configFile.write(configLines.join('\n').toStdString().c_str());
    } else {
        ok = false;
    }
    configFile.close();
    return ok;
}

void Calendar::checkDataFolder()
{
    if (dataFolderPath.isNull()) {
        dataFolderPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        dataFolder = QDir(dataFolderPath);
        calendarFolderPath = dataFolderPath + "/calendar/";

        QStringList splits = dataFolderPath.split('/');
        splits.removeLast();
        appdataPath = splits.join('/');
    }

    
    if (!dataFolder.exists())
        QDir(appdataPath).mkdir("Plume");

    if (!QDir(calendarFolderPath).exists())
        dataFolder.mkdir("calendar");
    
    if (!QFileInfo(calendarFolderPath + "config.txt").isFile())
        QFile(calendarFolderPath + "config.txt").open(QFile::WriteOnly);

    if (!QFileInfo(calendarFolderPath + "urls").isFile())
        QFile(calendarFolderPath + "urls").open(QFile::WriteOnly);

    if (!QDir(calendarFolderPath + "ics").exists())
        QDir(calendarFolderPath).mkdir("ics");
}


QStringList Calendar::getICSUrls()
{
    QFile urlsFile(calendarFolderPath + "urls");
    QStringList urls;
    if (urlsFile.open(QFile::ReadOnly)) {
        QString rawUrls = urlsFile.readAll();
        if (!rawUrls.isNull())
            urls = rawUrls.split('\n');
    }
    return urls;
}

QStringList Calendar::getICSFilePaths()
{
    return QDir(calendarFolderPath + "ics").entryList(QDir::Filter::Files);
}

void Calendar::loadRemoteICS(QString url)
{
    if (url.isEmpty()) return;

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkReply *reply = network.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (!reply->errorString().isNull()) {
            QByteArray replyContent = reply->readAll();
            loadICS(replyContent);
        }
        reply->deleteLater();
    });
}

bool intersect(Timestamp ts1, Timestamp ts2)
{
    QDateTime dts1 = ts1.first.addSecs(1);
    QDateTime dte1 = dts1.addSecs(ts1.second - 2);
    QDateTime dts2 = ts2.first;
    QDateTime dte2 = dts2.addSecs(ts2.second);
    return (dts1 < dts2 && dts2 < dte1)
        || (dts1 < dte2 && dte2 < dte1)
        || (dts2 < dts1 && dts1 < dte2)
        || (dts2 < dte1 && dte1 < dte2);
}

QList<Event *> intersections(QHash<Event *,Timestamp> pairs, Timestamp t)
{
    QList<Event *> events;
    for (Event *e : pairs.keys()) {
        if (intersect(pairs[e], t)) {
            events.append(e);
            e->reduce();
        }
    }
    return events;
}

void Calendar::loadICS(QString content)
{
    icalcomponent *cal = icalparser_parse_string(content.toStdString().c_str());
    if (icalcomponent_is_valid(cal)) {
        icalcomponent *c;
        for(c = icalcomponent_get_first_component(cal, ICAL_ANY_COMPONENT) ;
            c != 0 ;
            c = icalcomponent_get_next_component(cal, ICAL_ANY_COMPONENT))
        {
            icaltimetype time = icalcomponent_get_dtstart(c);
            QDateTime datetime(QDate(time.year, time.month, time.day), QTime(time.hour, time.minute, time.second));
            datetime.toTimeZone(timezone);
            QDate date = datetime.date();
            if (!components.contains(date)) {
                components[date] = QList<QPair<QDateTime, icalcomponent *>>();
            }
            components[date].append(QPair<QDateTime, icalcomponent *>(datetime, icalcomponent_clone(c)));   
        }

        loadEvents();
    }
    icalcomponent_free(cal);
}

void Calendar::loadEvents()
{
    events.clear();
    for (auto eventPair : eventList) {
        eventPair.first->deleteLater();
    }
    eventList.clear();

    QList<Event *> actualEvents;
    QDate date = spanStart.date();
    for (quint8 i = 0 ; i < spanDuration ; i++) {
        for (auto componentPair : components[date]) {
            QDateTime datetime = componentPair.first;
            icalcomponent *c = componentPair.second;
            
            QTime time = datetime.time();
            qint32 columnPos = (date.day() - spanStart.date().day()) * columnWidth + 2;
            qint32 rowPos = ((double)time.hour() + (double)time.minute() / 60 + (double)time.second() / 3600 + 1.0) * rowHeight + 2;
            double duration = icalcomponent_get_span(c).end - icalcomponent_get_span(c).start;
            QSize size = QSize(columnWidth - 4, duration / 3600.0 * rowHeight - 4);
            
            Event *event = new Event(datetime, c, eventField);
            if (!events.contains(date))
                events[date] = QList<QPair<Event *, Timestamp>>();
            events[date].append(QPair<Event *, Timestamp>(event, Timestamp(datetime, duration)));

            event->setFixedSize(size);
            event->move(columnPos, rowPos);
            actualEvents.append(event);

            eventList.append(QPair<Event *, Timestamp>(event, Timestamp(datetime, duration)));
            
            QObject::connect(event, &Event::clicked, [c, datetime, duration, this]() {
                ep = new EventPage(c, datetime, duration, this);
                contentHeader->hide();
                scroll->hide();
                lay->addWidget(ep);
                QObject::connect(ep, &EventPage::closed, [this]() {
                    contentHeader->show();
                    scroll->show();
                    ep->hide();
                    ep->deleteLater();
                    ep = nullptr;
                });
            });
        }

        date = date.addDays(1);
    }

    for (QDate date : events.keys()) {
        QHash<Event *, Timestamp> eventsTimestamps;
        for (auto pair : events[date]) {
            QList<Event *> intersects = intersections(eventsTimestamps, pair.second);
            eventsTimestamps[pair.first] = pair.second;
            if (intersects.size() == 1 && intersects[0]->order > 0) {
                pair.first->order = intersects[0]->order - 1;
                pair.first->reduce();
                for (Event *e : intersects) {
                    e->reduce(-1);
                }
            } else {
                pair.first->order = intersects.size();
                pair.first->reduce(intersects.size());
            }
        }
    }

    for (Event *e : actualEvents)
        e->show();
}

void Calendar::updateSizes()
{
    QSize contentSize = content->size();
    int width = contentSize.width();
    columnWidth = (width - 30) / spanDuration;
    offset = 30 + (width - 30) % spanDuration;


    hourContainer->setFixedHeight(rowHeight);

    dayLabels[0]->setFixedWidth(offset);
    for (qint8 i = 1 ; i < dayLabels.size() ; i++) {
        dayLabels[i]->setFixedWidth(columnWidth);
    }

    for (QPair<Event *, Timestamp> pair : eventList) {
        Timestamp ts = pair.second;
        Event *e = pair.first;
        QTime time = ts.first.time();
        qint32 columnPos = (ts.first.date().day() - spanStart.date().day()) * columnWidth + 2;
        qint32 rowPos = ((double)time.hour() + (double)time.minute() / 60 + (double)time.second() / 3600 + 1.0) * rowHeight + 2;
        qint32 width = (columnWidth - 4) / e->reduceFactor;
        qint32 height = ts.second / 3600.0 * rowHeight - 4;

        e->setFixedSize(width, height);
        e->move(columnPos + width * e->order, rowPos);
    }
}


void Calendar::showEvent(QShowEvent *)
{
    updateSizes();
}

void Calendar::resizeEvent(QResizeEvent *)
{
    updateSizes();
}