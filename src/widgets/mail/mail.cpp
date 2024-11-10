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
    loadUi();
}

void Mail::loadUi() {
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    header = new QWidget(this);
    header->setProperty("class", "mail-header");
    header->setFixedHeight(60);
    QHBoxLayout *headerLay = new QHBoxLayout(header);
    headerLay->setContentsMargins(20, 10, 20, 10);
    headerLay->setSpacing(0);

    title = new QLabel("Mails", header);

    /*search = new HeaderButton(HeaderButton::Type::Today, header);
    settings = new HeaderButton(HeaderButton::Type::Settings, header);

    QObject::connect(today, &HeaderButton::clicked, [this]() {
        resetSpan();
        loadEvents();
    });
    QObject::connect(settings, &HeaderButton::clicked, this, &Calendar::openSettings);*/

    spacer = new QWidget(header);
    
    /*headerLay->addWidget(search, 0, Qt::AlignVCenter);
    headerLay->addSpacing(20);*/
    headerLay->addWidget(title, 0, Qt::AlignVCenter);
    headerLay->addWidget(spacer);
    //headerLay->addWidget(settings, 0, Qt::AlignVCenter);


    QWidget *body = new QWidget(this);
    QVBoxLayout *bodyLay = new QVBoxLayout(body);
    bodyLay->setContentsMargins(0, 0, 0, 0);
    bodyLay->setSpacing(0);

    contentHeader = new QWidget(body);
    contentHeader->setFixedHeight(30);
    QHBoxLayout *contentHeaderLay = new QHBoxLayout(contentHeader); 

    received = new QLabel("Received", contentHeader);
    folders = new QLabel("Folders", contentHeader);
    other = new QLabel("Other", contentHeader);
    contentHeaderLay->addSpacing(20);
    contentHeaderLay->addWidget(received, 0, Qt::AlignVCenter);
    contentHeaderLay->addSpacing(50);
    contentHeaderLay->addWidget(folders, 0, Qt::AlignVCenter);
    contentHeaderLay->addSpacing(50);
    contentHeaderLay->addWidget(other, 0, Qt::AlignVCenter);

    contentHeaderLay->addStretch();
    contentHeaderLay->setContentsMargins(0, 0, 0, 0);
    contentHeaderLay->setSpacing(0);

    scroll = new QScrollArea(this);
    scroll->setProperty("class", "mail-scroll");
    scroll->setAttribute(Qt::WA_StyledBackground);
    
    content = new QWidget(scroll);
    contentLay = new QVBoxLayout(content);

    contentLay->setContentsMargins(0, 0, 0, 0);
    contentLay->setSpacing(0);

    scroll->setWidget(content);
    scroll->setContentsMargins(0, 0, 0, 0);
    scroll->setWidgetResizable(true);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    bodyLay->addWidget(contentHeader);
    bodyLay->addWidget(scroll);

    lay->addWidget(header);
    lay->addWidget(body);

    setProperty("class", "mail");
    setAttribute(Qt::WA_StyledBackground);
}

void Mail::loadMailPreviews() {
    for (MailData &m : login.mails) {
        MailPreview *preview = new MailPreview(m, content);
        contentLay->addWidget(preview);
    }
    contentLay->addStretch();
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

void Mail::foldersLoaded(QHash<qint64, Folder>& folders) {
    /*qDebug() << "Folders loaded:";
    for (Folder f : folders) {
        qDebug() << f.name << f.path << f.id << f.uuid << f.color << f.deletable;
    }*/
}

void Mail::contactsLoaded(QHash<qint64, Contact>& contacts) {
    contactsLoad = true;
    if (mailsLoad) {
        loadMailPreviews();
    }

    /*qDebug() << "Contacts loaded:";
    for (Contact c : contacts) {
        qDebug() << c.fileAsStr << c.firstName << c.lastName << c.fullName << c.email << c.id << c.l << c.d << c.rev;
    }*/
}

void Mail::mailsLoaded(QList<MailData> mails) {
    mailsLoad = true;
    if (contactsLoad) {
        loadMailPreviews();
    }

    /*qDebug() << "Mails loaded:";
    for (MailData &m : mails) {
        qDebug() << m.subject << m.preview << m.sf << m.timestamp << m.id << m.u << m.n;
        for (Sender s : m.senders) {
            qDebug() << "    " << s.email << s.lastName << s.fullName << s.t;
        }
        for (Receiver r : m.receivers) {
            qDebug() << "    " << r.id << r.l << r.timestamp;
        }
    }*/
}