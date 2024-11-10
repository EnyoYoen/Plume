#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

#include "mailpreview.h"

#include "login.h"

class Mail : public QWidget
{
    Q_OBJECT

public:
    Mail(QWidget *parent);

private slots:
    void loginEnded(Login::Status status);
    void connectionEnded(Login::Status status);

    void foldersLoaded(QHash<qint64, Folder>& folders);
    void contactsLoaded(QHash<qint64, Contact>& contacts);
    void mailsLoaded(QList<MailData> mails);

private:
    void loadUi();
    void loadMailPreviews();

    QVBoxLayout *lay = nullptr;
    QWidget *header = nullptr;
    QLabel *title = nullptr;
    QWidget *spacer = nullptr;
    /*HeaderButton *search = nullptr;
    HeaderButton *settings = nullptr;*/
    QWidget *contentHeader = nullptr;
    QLabel *received = nullptr;
    QLabel *folders = nullptr;
    QLabel *other = nullptr;
    QScrollArea *scroll = nullptr;
    QWidget *content = nullptr;
    QVBoxLayout *contentLay = nullptr;

    Login login;
    bool contactsLoad = false;
    bool mailsLoad = false;
};