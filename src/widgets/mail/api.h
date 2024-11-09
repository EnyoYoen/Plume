#pragma once

#include <QString>
#include <QList>

namespace Api {

struct Folder {
    QString name;
    QString path;
    QString uuid;
    qint64 id;
    qint64 color;
    bool deletable;
};

struct Contact {
    QString fileAsStr;
    QString firstName;
    QString lastName;
    QString fullName;
    QString email;
    qint64 id;
    qint64 l;
    qint64 d;
    qint64 rev;
};

struct Sender {
    QString email;
    QString lastName;
    QString fullName;
    QString t;
};

struct Receiver {
    qint64 id;
    qint64 l;
    qint64 timestamp;
};

struct MailData {
    QList<Sender> senders;
    QList<Receiver> receivers;
    QString subject;
    QString preview;
    QString sf; // A string timestamp?
    qint64 timestamp;
    qint64 id;
    qint64 u;
    qint64 n;
};

} // namespace Api