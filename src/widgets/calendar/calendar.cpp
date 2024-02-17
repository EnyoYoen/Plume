#include "calendar.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QStandardPaths>
#include <QFileInfo>

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
{
    loadUI();
    loadCalendars();
}

void Calendar::loadUI()
{
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    header = new QWidget(this);
    header->setFixedHeight(30);
    QHBoxLayout *headerLay = new QHBoxLayout(header); 

    for (quint8 i = 0 ; i < 8 ; i++) {
        QLabel *day = new QLabel(days[i], header);
        if (i == 0) {
            day->setFixedWidth(offset);
        } else {
            day->setFixedWidth(columnWidth);
            day->setAlignment(Qt::AlignCenter);
            day->setProperty("class", "calendar-day");
        }
        headerLay->addWidget(day, 0, Qt::AlignVCenter);
    }

    headerLay->addStretch();
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(0);

    scroll = new QScrollArea(this);
    scroll->setProperty("class", "calendar-scroll");
    scroll->setAttribute(Qt::WA_StyledBackground);
    
    QWidget *content = new QWidget(scroll);
    QHBoxLayout *contentLay = new QHBoxLayout(content);

    QWidget *hours = new QWidget(content);
    hours->setFixedWidth(offset);
    QVBoxLayout *hoursLay = new QVBoxLayout(hours);

    for (quint8 i = 0 ; i < 25 ; i++) {
        QWidget *hourContainer = new QWidget(hours);
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

    scroll->setWidget(content);
    scroll->setContentsMargins(0, 0, 0, 0);
    scroll->setWidgetResizable(true);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lay->addWidget(header);
    lay->addWidget(scroll);

    setProperty("class", "calendar");
    setAttribute(Qt::WA_StyledBackground);
}

void Calendar::loadCalendars()
{
    spanDuration = 7;          // TODO : add a config file to change the
    spanType = SpanType::Week; // span type and duration persistently

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
    spanEnd = QDateTime(spanStart.date().addDays(spanDuration - 1), QTime(23, 59, 59));

    checkDataFolder();

    for (QString url : getICSUrls())
        loadRemoteICS(url);

    for (QString filename : getICSFilePaths()) {
        QFile file(calendarFolderPath + "ics/" + filename);
        file.open(QFile::ReadOnly);
        loadICS(file.readAll());
    }
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

void describe(icalcomponent *component)
{
    qDebug() << "--- Start describing ---";
    qDebug() << icalcomponent_kind_to_string(icalcomponent_isa(component));
    qDebug() << icalcomponent_get_comment(component);
    qDebug() << QString(icalcomponent_get_description(component)).remove('\n');
    qDebug() << icalcomponent_get_dtstart(component).day << " " << icalcomponent_get_dtstart(component).hour << " " << icalcomponent_get_dtstart(component).minute << " " << icalcomponent_get_dtstart(component).second;
    qDebug() << icalcomponent_get_dtend(component).day << " " << icalcomponent_get_dtend(component).hour << " " << icalcomponent_get_dtend(component).minute << " " << icalcomponent_get_dtend(component).second;
    qDebug() << icalcomponent_get_duration(component).seconds;
    qDebug() << icalcomponent_get_location(component);
    qDebug() << icalcomponent_get_span(component).start << " " << icalcomponent_get_span(component).end << " " << float(icalcomponent_get_span(component).end - icalcomponent_get_span(component).start) / 3600.0;
    qDebug() << icalcomponent_get_status(component);
    qDebug() << icalcomponent_get_summary(component);
    qDebug() << icalcomponent_get_uid(component);
    qDebug() << "--- End describing ---\n";
}

void Calendar::loadICS(QString content)
{
    icalcomponent *cal = icalparser_parse_string(content.toStdString().c_str());
    if (icalcomponent_is_valid(cal)) {
        //describe(cal);
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
            components[date].append(QPair<QDateTime, icalcomponent *>(datetime, /*icalcomponent_new_clone(*/c/*)*/));

            //qDebug() << datetime << " " << spanStart << " " << QDateTime(spanStart.date().addDays(spanDuration), QTime(23, 59, 59)) << " " << (datetime >= spanStart) << " " << (datetime <= QDateTime(spanStart.date().addDays(spanDuration), QTime(23, 59, 59)));
            if (datetime >= spanStart && datetime <= spanEnd) {
                QTime time = datetime.time();
                qint32 columnPos = (date.day() - spanStart.date().day()) * columnWidth + 2;
                qint32 rowPos = ((double)time.hour() + (double)time.minute() / 60 + (double)time.second() / 3600 + 1.0) * rowHeight + 2;
                double duration = icalcomponent_get_span(c).end - icalcomponent_get_span(c).start;
                QSize size = QSize(columnWidth - 4, duration / 3600.0 * rowHeight - 4);
                //qDebug() << size << QPoint(columnPos, rowPos);
                Event *event = new Event(datetime, c, eventField);
                event->setFixedSize(size);
                event->move(columnPos, rowPos);
                QObject::connect(event, &Event::clicked, [c, datetime, duration, this]() {
                    ep = new EventPage(c, datetime, duration, this);
                    header->hide();
                    scroll->hide();
                    lay->addWidget(ep);
                    QObject::connect(ep, &EventPage::closed, [this]() {
                        header->show();
                        scroll->show();
                        ep->hide();
                        ep->deleteLater();
                        ep = nullptr;
                    });
                });
            }
        }
    }
    //icalcomponent_free(cal);
}