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

    QWidget *header = new QWidget(this);
    header->setFixedHeight(30);
    QHBoxLayout *headerLay = new QHBoxLayout(header); 

    for (quint8 i = 0 ; i < 8 ; i++) {
        QLabel *day = new QLabel(days[i], header);
        if (i == 0) {
            day->setFixedWidth(30);
        } else {
            day->setProperty("class", "calendar-day");
        }
        headerLay->addWidget(day);
    }

    headerLay->setContentsMargins(0, 0, 0, 0);

    scroll = new QScrollArea(this);
    scroll->setProperty("class", "calendar-scroll");
    
    QWidget *content = new QWidget(scroll);
    QHBoxLayout *contentLay = new QHBoxLayout(content);

    QWidget *hours = new QWidget(content);
    hours->setFixedWidth(30);
    QVBoxLayout *hoursLay = new QVBoxLayout(hours);

    for (quint8 i = 1 ; i < 25 ; i++) {
        QLabel *hour = new QLabel(QString::number(i) + "h", hours);
        hour->setProperty("class", "calendar-hour");
        hour->setFixedHeight(60);
        hoursLay->addWidget(hour, 0, Qt::AlignHCenter);
    }

    hoursLay->setContentsMargins(0, 0, 0, 0);

    contentLay->addWidget(hours);
    contentLay->addStretch();
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
    spanDuration = 5;               // TODO : add a config file to change the
    spanType = SpanType::NoWeekend; // span type and duration persistently

    spanStart = QDate::currentDate();
    switch (spanType)
    {
        case SpanType::Month:
            spanStart = spanStart.addDays(1 - spanStart.day());
            break;

        default: // NoWeekend or Week
            spanStart = spanStart.addDays(1 - spanStart.dayOfWeek());
    }

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
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkReply *reply = network.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        QByteArray replyContent = reply->readAll();
        loadICS(replyContent);
    });
}

void Calendar::loadICS(QString content)
{
    
}