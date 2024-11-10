#pragma once

#include <QObject>
#include <QString>
#include <QHash>
#include <QNetworkAccessManager>

#include "config.h"
#include "api.h"

using namespace Api;

class Login : public QObject {

    Q_OBJECT

public:
    enum class Status {
        SUCCESS,
        NO_LOGIN,
        NO_PASSWORD,
        NO_CREDENTIALS,
        NO_MAIL,
        INVALID_CREDENTIALS,
        REQUEST_ERROR,
        LOGIN_ERROR
    };

    Login(QObject *parent = nullptr);

    void startLogin();
    void connectZM();

    void getFolders();
    void getContacts();
    void getMails(qint64 offset/*, qint64 lastId, qint64 folderId, quint8 limit*/); // TODO: add parameters for folder and limit

    QHash<qint64, Folder> folders;
    QHash<qint64, Contact> contacts;
    QList<MailData> mails;

signals:
    void loginEnd(Status status);
    void connected(Status status);

    void foldersLoaded(QHash<qint64, Folder>& folders);
    void contactsLoaded(QHash<qint64, Contact>& contacts);
    void mailsLoaded(QList<MailData> mails);

private:
    QByteArray joinCookies();
    void setHeaders(QNetworkRequest &request, bool cookies = false, bool post = false, QString host = "zmail.insa-lyon.fr");

    void sendLoginInit();
    void sendLoginData();

    void sendLoginInitZM();
    void sendLoginDataZM();

    MailConfig config;
    QNetworkAccessManager network;

    QString insaSSO;
    QString insaSSOpdata;
    QString lt;
    QString execution;
    QString agimus;
    QString zmCRSF;
    QHash<QString, QString> cookies;
};