#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QDir>

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
    QDate spanStart;
    qint64 spanDuration;
    SpanType spanType;

    QVBoxLayout *lay = nullptr;
    QScrollArea *scroll = nullptr;
};