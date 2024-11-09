#include "config.h"

#include <QStandardPaths>
#include <QFileInfo>

void CalendarConfig::load()
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
                if (splits.size() == 2) {
                    bool ok = true;
                    int result = splits[1].toInt(&ok);
                    if (ok && (spanType <= SpanType::Month && spanType > SpanType::None)) {
                        spanType = (SpanType)result;
                    }
                }
            } else if (line.startsWith("disabledCalendars")) {
                QStringList splits = line.split(':');
                if (splits.size() == 2 && splits[1].startsWith('[') && splits[1].endsWith(']')) {
                    QStringList names = splits[1].mid(1, splits[1].size() - 2).split(',');
                    for (QString name : names) {
                        if (name.startsWith('(') && name.endsWith(')')) {
                            QStringList namePair = name.mid(1, name.size() - 2).split('/');
                            if (namePair.size() == 2) {
                                disabledCalendars[CalendarName(namePair[0].isEmpty() ? QString() : namePair[0],
                                    namePair[1].isEmpty() ? QUrl() : QUrl::fromPercentEncoding(namePair[1].toUtf8()))] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    spanDuration = typeDuration[spanType];
}

bool CalendarConfig::add(QString name, QVariant value)
{
    checkDataFolder();

    bool found = false;
    QStringList configLines;
    QString configLine = name + ":" + value.toString();
    QFile configFile(calendarFolderPath + "config.txt");
    if (configFile.open(QFile::ReadOnly)) {
        QString config = configFile.readAll();
        configLines = config.split('\n', Qt::SplitBehaviorFlags::SkipEmptyParts);
        for (quint64 i = 0 ; i < configLines.size() ; i++) {
            if (configLines[i].startsWith(name)) {
                configLines[i] = configLine;
                found = true;
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

bool CalendarConfig::add(QString name, QHash<CalendarName, bool> value)
{
    checkDataFolder();

    QString configLine = name + ":[";
    qsizetype i = 0;
    for (CalendarName name : value.keys()) {
        i++;
        configLine.append("(" + name.first + "/" + QUrl::toPercentEncoding(name.second.url()) + ")");
        if (i < value.size()) {
            configLine.append(',');
        }
    }
    configLine.append(']');

    bool found = false;
    QStringList configLines;
    QFile configFile(calendarFolderPath + "config.txt");
    if (configFile.open(QFile::ReadOnly)) {
        QString config = configFile.readAll();
        configLines = config.split('\n', Qt::SplitBehaviorFlags::SkipEmptyParts);
        for (quint64 i = 0 ; i < configLines.size() ; i++) {
            if (configLines[i].startsWith(name)) {
                configLines[i] = configLine;
                found = true;
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


void CalendarConfig::checkDataFolder()
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

QFile CalendarConfig::getICSFile(QString name) {
    checkDataFolder();

    return QFile(calendarFolderPath + "ics/" + name);
}

QStringList CalendarConfig::getICSUrls()
{
    checkDataFolder();

    QFile urlsFile(calendarFolderPath + "urls");
    QStringList urls;
    if (urlsFile.open(QFile::ReadOnly)) {
        QString rawUrls = urlsFile.readAll();
        if (!rawUrls.isNull())
            urls = rawUrls.split('\n');
    }
    return urls;
}

QStringList CalendarConfig::getICSFilePaths()
{
    checkDataFolder();

    return QDir(calendarFolderPath + "ics").entryList(QDir::Filter::Files);
}
