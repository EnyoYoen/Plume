#include "mail.h"

Mail::Mail(QWidget *parent)
    : QWidget(parent)
{
    QObject::connect(&login, &Login::loginEnd, this, &Mail::loginEnded);
    QObject::connect(&login, &Login::connected, this, &Mail::connectionEnded);
    QObject::connect(&login, &Login::foldersLoaded, this, &Mail::foldersLoaded);
    QObject::connect(&login, &Login::contactsLoaded, this, &Mail::contactsLoaded);
    QObject::connect(&login, &Login::mailsLoaded, this, &Mail::mailsLoaded);
    login.startLogin();

    lay = new QHBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    setProperty("class", "mail");
    setAttribute(Qt::WA_StyledBackground);
}

void Mail::loginEnded(Login::Status status) {
    qDebug() << "Login ended with status:" << (int)status;

    if (status == Login::Status::SUCCESS) {
        login.connectZM();
    }
}

void Mail::connectionEnded(Login::Status status) {
    qDebug() << "Connection ended with status:" << (int)status;

    if (status == Login::Status::SUCCESS) {
        login.getFolders();
        login.getContacts();
        login.getMails(0);
    }
}

void Mail::foldersLoaded(QList<Folder> folders) {
    qDebug() << "Folders loaded:";
    for (Folder f : folders) {
        qDebug() << f.name << f.path << f.id << f.uuid << f.color << f.deletable;
    }
}

void Mail::contactsLoaded(QList<Contact> contacts) {
    qDebug() << "Contacts loaded:";
    for (Contact c : contacts) {
        qDebug() << c.fileAsStr << c.firstName << c.lastName << c.fullName << c.email << c.id << c.l << c.d << c.rev;
    }
}

void Mail::mailsLoaded(QList<MailData> mails) {
    qDebug() << "Mails loaded:";
    for (MailData &m : mails) {
        qDebug() << m.subject << m.preview << m.sf << m.timestamp << m.id << m.u << m.n;
        for (Sender s : m.senders) {
            qDebug() << "    " << s.email << s.lastName << s.fullName << s.t;
        }
        for (Receiver r : m.receivers) {
            qDebug() << "    " << r.id << r.l << r.timestamp;
        }
    }
}