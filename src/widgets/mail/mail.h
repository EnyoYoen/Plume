#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "login.h"

class Mail : public QWidget
{
    Q_OBJECT

public:
    Mail(QWidget *parent);

private slots:
    void loginEnded(Login::Status status);
    void connectionEnded(Login::Status status);

    void foldersLoaded(QList<Folder> folders);
    void contactsLoaded(QList<Contact> contacts);
    void mailsLoaded(QList<MailData> mails);

private:
    QHBoxLayout *lay = nullptr;

    Login login;
};