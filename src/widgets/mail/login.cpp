#include "login.h"

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QThread>

Login::Login(QObject *parent)
    : QObject(parent)
{
    config.load();
}


QByteArray Login::joinCookies() {
    QString cookie;
    for (auto it = cookies.begin(); it != cookies.end(); ++it) {
        cookie += it.key() + "=" + it.value() + "; ";
    }
    return cookie.toUtf8();
}

void Login::setHeaders(QNetworkRequest &request, bool cookies, bool post, QString host) {
    request.setRawHeader("Accept-Charset", "UTF-8");
    request.setRawHeader("Connection", "Keep-Alive");
    //request.setRawHeader("Accept-Encoding", "gzip"); // TODO: add gzip support
    request.setRawHeader("Host", host.toUtf8());
    request.setRawHeader("User-Agent", "Mozilla/5.0 INK");
    if (cookies) {
        request.setRawHeader("Cookie", joinCookies());
    }
    if (post) {
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    }
}


void Login::startLogin() {
    if (config.login.isEmpty() || config.password.isEmpty()) {
        QDialog dialog(nullptr);
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel("Enter your credentials"));


        QLineEdit *loginEdit = new QLineEdit(&dialog);
        QLineEdit *passwordEdit = new QLineEdit(&dialog);
        QString loginLabel = QString("Login");
        QString passwordLabel = QString("Password");

        form.addRow(loginLabel, loginEdit);
        form.addRow(passwordLabel, passwordEdit);

        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        // Show the dialog as modal
        if (dialog.exec() == QDialog::Accepted) {
            QString loginForm = loginEdit->text();
            QString passwordForm = passwordEdit->text();

            if (!loginForm.isEmpty()) {
                config.add("login", loginForm);
            }
            if (!passwordForm.isEmpty()) {
                config.add("password", passwordForm);
            }

            if (loginForm.isEmpty() || passwordForm.isEmpty()) {
                if (loginForm.isEmpty()) {
                    emit loginEnd(Status::NO_LOGIN);
                } else if (passwordForm.isEmpty()) {
                    emit loginEnd(Status::NO_PASSWORD);
                }
            } else {
                config.login = loginForm;
                config.password = passwordForm;

                sendLoginInit();
            }
        } else {
            if (config.login.isEmpty() && config.password.isEmpty()) {
                emit loginEnd(Status::NO_CREDENTIALS);
            } else {
                if (config.login.isEmpty()) {
                    emit loginEnd(Status::NO_LOGIN);
                } else if (config.password.isEmpty()) {
                    emit loginEnd(Status::NO_PASSWORD);
                }
            }
        }
    } else {
        sendLoginInit();
    }
}

void Login::sendLoginInit() {
    QNetworkRequest request(QUrl("https://login.insa-lyon.fr/cas/login?gateway=true"));
    setHeaders(request, false, false, "login.insa-lyon.fr");

    QNetworkReply *reply = network.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode == 302) {
            emit loginEnd(Status::SUCCESS);
        } else if (statusCode != 200) {
            emit loginEnd(Status::REQUEST_ERROR);
        }

        if (reply->error() != QNetworkReply::NoError) {
            emit loginEnd(Status::REQUEST_ERROR);
        } else {
            bool found = false;
            QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
            for (const QNetworkReply::RawHeaderPair &header : headers) {
                if (header.first.toLower() == "set-cookie") {
                    QString cookieList = header.second;
                    qsizetype equalIndex;
                    qsizetype colonIndex;
                    for (const QString &cookie : cookieList.split("\n")) {
                        equalIndex = cookie.indexOf("=");
                        colonIndex = cookie.indexOf(";");
                        if (equalIndex == -1) {
                            continue;
                        }
                        if (colonIndex != -1) {
                            colonIndex = cookie.size();
                        }
                        cookies.insert(cookie.mid(0, equalIndex), cookie.mid(equalIndex + 1, colonIndex - equalIndex - 1));
                    }
                    found = true;
                }
            }

            if (!found) {
                emit loginEnd(Status::REQUEST_ERROR);
            } else {
                QString replyPage = QString(reply->readAll());
                QString inputLT = replyPage.mid(replyPage.indexOf("<input type=\"hidden\" name=\"lt\" value=\"") + 38);
                lt = inputLT.mid(0, inputLT.indexOf("\""));
                QString inputExecution = replyPage.mid(replyPage.indexOf("<input type=\"hidden\" name=\"execution\" value=\"") + 45);
                execution = inputExecution.mid(0, inputExecution.indexOf("\""));

                sendLoginData();
            }
        }
        reply->deleteLater();
    });
}

void Login::sendLoginData() {
    QNetworkRequest request(QUrl("https://login.insa-lyon.fr/cas/login?gateway=true"));
    setHeaders(request, true, true, "login.insa-lyon.fr");

    QString data = "username=" + QUrl::toPercentEncoding(config.login) + "&password=" + QUrl::toPercentEncoding(config.password) + "&execution=" + execution + "&_eventId=submit&rememberMe=true&lt=" + lt;
    QNetworkReply *reply = network.post(request, data.toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            emit loginEnd(Status::LOGIN_ERROR);
        }

        if (reply->error() != QNetworkReply::NoError) {
            emit loginEnd(Status::LOGIN_ERROR);
        } else {
            bool found = false;
            QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
            for (const QNetworkReply::RawHeaderPair &header : headers) {
                if (header.first.toLower() == "set-cookie") {
                    QString cookieList = header.second;
                    qsizetype equalIndex;
                    qsizetype colonIndex;
                    for (const QString &cookie : cookieList.split("\n")) {
                        equalIndex = cookie.indexOf("=");
                        colonIndex = cookie.indexOf(";");
                        if (equalIndex == -1) {
                            continue;
                        }
                        if (colonIndex != -1) {
                            colonIndex = cookie.size();
                        }
                        cookies.insert(cookie.mid(0, equalIndex), cookie.mid(equalIndex + 1, colonIndex - equalIndex - 1));
                    }
                    found = true;
                }
            }

            if (found && cookies.contains("AGIMUS")) {
                agimus = cookies["AGIMUS"];

                emit loginEnd(Status::SUCCESS);
            } else {
                emit loginEnd(Status::LOGIN_ERROR);
            }
        }
        reply->deleteLater();
    });
}


void Login::connectZM() {
    if (config.mail.isEmpty()) {
        QDialog dialog(nullptr);
        QFormLayout form(&dialog);

        form.addRow(new QLabel("Enter your mail address"));

        QLineEdit *mailEdit = new QLineEdit(&dialog);
        QString mailLabel = QString("Mail");

        form.addRow(mailLabel, mailEdit);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            QString mailForm = mailEdit->text();

            if (!mailForm.isEmpty()) {
                config.add("mail", mailForm);
                config.mail = mailForm;
                sendLoginInitZM();
            } else {
                emit connected(Status::NO_MAIL);
            }
        } else {
            emit connected(Status::NO_MAIL);
        }
    } else {
        sendLoginInitZM();
    }
}

void Login::sendLoginInitZM() {
    QNetworkRequest request(QUrl("https://zmail.insa-lyon.fr/"));
    setHeaders(request);

    QNetworkReply *reply = network.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            emit connected(Status::REQUEST_ERROR);
        }

        if (reply->error() != QNetworkReply::NoError) {
            emit connected(Status::REQUEST_ERROR);
        } else {
            bool found = false;
            cookies.clear();
            QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
            for (const QNetworkReply::RawHeaderPair &header : headers) {
                if (header.first.toLower() == "set-cookie") {
                    QString cookieList = header.second;
                    qsizetype equalIndex;
                    qsizetype colonIndex;
                    for (const QString &cookie : cookieList.split("\n")) {
                        equalIndex = cookie.indexOf("=");
                        colonIndex = cookie.indexOf(";");
                        if (equalIndex == -1) {
                            continue;
                        }
                        if (colonIndex != -1) {
                            colonIndex = cookie.size();
                        }
                        cookies.insert(cookie.mid(0, equalIndex), cookie.mid(equalIndex + 1, colonIndex - equalIndex - 1));
                    }
                    found = true;
                }
            }

            if (!found) {
                emit connected(Status::REQUEST_ERROR);
            } else {
                QString replyPage = QString(reply->readAll());
                QString inputCRSF = replyPage.mid(replyPage.indexOf("<input type=\"hidden\" name=\"login_csrf\" value=\"") + 46);
                zmCRSF = inputCRSF.mid(0, inputCRSF.indexOf("\""));

                //qDebug() << zmCRSF << cookies;

                sendLoginDataZM();
            }
        }
        reply->deleteLater();
    });
}

void Login::sendLoginDataZM() {
    QNetworkRequest request(QUrl("https://zmail.insa-lyon.fr/"));
    setHeaders(request, true, true);

    QString data = "loginOp=login&login_csrf=" + zmCRSF + "&username=" + QUrl::toPercentEncoding(config.mail) + "&password=" + config.password + "&client=preferred";
    QNetworkReply *reply = network.post(request, data.toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            //qDebug() << statusCode;

            emit connected(Status::LOGIN_ERROR);

            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            //qDebug() << reply->errorString() << reply->error();

            emit connected(Status::LOGIN_ERROR);
        } else {
            bool found = false;
            QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
            for (const QNetworkReply::RawHeaderPair &header : headers) {
                if (header.first.toLower() == "set-cookie") {
                    QString cookieList = header.second;
                    qsizetype equalIndex;
                    qsizetype colonIndex;
                    for (const QString &cookie : cookieList.split("\n")) {
                        equalIndex = cookie.indexOf("=");
                        colonIndex = cookie.indexOf(";");
                        if (equalIndex == -1) {
                            continue;
                        }
                        if (colonIndex == -1) {
                            colonIndex = cookie.size();
                        }
                        cookies.insert(cookie.mid(0, equalIndex), cookie.mid(equalIndex + 1, colonIndex - equalIndex - 1));
                    }
                    found = true;
                }
            }

            if (found) {
                //qDebug() << cookies;

                emit connected(Status::SUCCESS);
            } else {
                //qDebug() << "No cookies found";

                emit connected(Status::LOGIN_ERROR);
            }
        }
        reply->deleteLater();
    });
}


void Login::getFolders() {
    QNetworkRequest request(QUrl("https://zmail.insa-lyon.fr/service/soap/GetFolderRequest"));
    setHeaders(request, true, true);

    QString data = "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\"><soap:Header><context xmlns=\"urn:zimbra\"><userAgent xmlns=\"\" name=\"ZimbraWebClient - FF123 (Linux)\" version=\"8.8.15_GA_4652\"/><session xmlns=\"\" id=\"13226541\"/><account xmlns=\"\" by=\"name\">" + config.mail + "</account><format xmlns=\"\" type=\"js\"/></context></soap:Header><soap:Body><GetFolderRequest xmlns=\"urn:zimbraMail\"><folder xmlns=\"\"/></GetFolderRequest></soap:Body></soap:Envelope>";
    QNetworkReply *reply = network.post(request, data.toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            return;
        } else {
            QByteArray resp = reply->readAll();

            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonArray jfolders = doc.object()["Body"].toObject()["GetFolderResponse"].toObject()["folder"].toArray()[0].toObject()["folder"].toArray();
            if (jfolders.size() > 0) {
                for (const QJsonValue &folder : jfolders) {
                    Folder f;
                    f.name = folder.toObject()["name"].toString();
                    f.path = folder.toObject()["absFolderPath"].toString();
                    f.uuid = folder.toObject()["uuid"].toString();
                    f.id = folder.toObject()["id"].toVariant().toLongLong();
                    f.color = folder.toObject()["color"].toVariant().toLongLong();
                    f.deletable = folder.toObject()["deletable"].toBool();
                    this->folders.insert(f.id, f);
                }
                emit foldersLoaded(folders);
            }
        }
        reply->deleteLater();
    });
}

void Login::getContacts() {
    QNetworkRequest request(QUrl("https://zmail.insa-lyon.fr/home/" + config.mail + "/Contacts?fmt=cf&t=2&all=all"));
    setHeaders(request, true);

    QNetworkReply *reply = network.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            return;
        } else {
            QString resp = reply->readAll(); 
            
            for (QString rawContact : resp.split("")) {
                QStringList contact = rawContact.split("");
                Contact c;
                for (qsizetype i = 0 ; i < contact.size() ; i+=2) {
                    if (contact[i] == "fileAsStr") {
                        c.fileAsStr = contact[i+1];
                    } else if (contact[i] == "firstName") {
                        c.firstName = contact[i+1];
                    } else if (contact[i] == "lastName") {
                        c.lastName = contact[i+1];
                    } else if (contact[i] == "fullName") {
                        c.fullName = contact[i+1];
                    } else if (contact[i] == "email") {
                        c.email = contact[i+1];
                    } else if (contact[i] == "id") {
                        c.id = contact[i+1].toLongLong();
                    } else if (contact[i] == "l") {
                        c.l = contact[i+1].toLongLong();
                    } else if (contact[i] == "d") {
                        c.d = contact[i+1].toLongLong();
                    } else if (contact[i] == "rev") {
                        c.rev = contact[i+1].toLongLong();
                    }
                }
                contacts.insert(c.id, c);
            }
            emit contactsLoaded(contacts);
        }
        reply->deleteLater();
    });
}

void Login::getMails(qint64 offset/*, qint64 lastId, qint64 folderId, quint8 limit*/) {
    QNetworkRequest request(QUrl("https://zmail.insa-lyon.fr/service/soap/SearchRequest"));
    setHeaders(request, true, true);

    QString data = "{\"Header\":{"
                        "\"context\":{"
                            "\"_jsns\":\"urn:zimbra\","
                            "\"userAgent\":{"
                                "\"name\":\"ZimbraWebClient - FF132 (Linux)\","
                                "\"version\":\"8.8.15_GA_4652\"},"
                                /*"\"session\":{"
                                    "\"_content\":13230774,"
                                    "\"id\":13230774},"*/
                                "\"account\":{"
                                    "\"_content\":\"" + config.mail + "\",\"by\":\"name\"}}},"
                     "\"Body\":{"
                        "\"SearchRequest\":{"
                            "\"_jsns\":\"urn:zimbraMail\","
                            "\"sortBy\":\"dateDesc\","
                            "\"header\":["
                                "{\"n\":\"List-ID\"},"
                                "{\"n\":\"X-Zimbra-DL\"},"
                                "{\"n\":\"IN-REPLY-TO\"}],"
                            "\"tz\":{"
                                "\"id\":\"Europe/Brussels\"},"
                            "\"locale\":{"
                                "\"_content\":\"fr_FR\"},"
                            /*"\"cursor\":{"
                                "\"id\":\"-33520\","
                                "\"sortVal\":\"1729152607000\"},"*/
                            "\"offset\":" + QString::number(offset) + ","
                            "\"limit\":50," // TODO: add limit
                            "\"query\":\"in:inbox\"," // TODO: add folder choice
                            "\"types\":\"conversation\","
                            "\"recip\":\"0\","
                            "\"fullConversation\":1,"
                            "\"needExp\":1}}}";
    QNetworkReply *reply = network.post(request, data.toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200 && statusCode != 302) {
            return;
        }

        if (reply->error() != QNetworkReply::NoError) {
            return;
        } else {
            QByteArray resp = reply->readAll();

            QJsonDocument doc = QJsonDocument::fromJson(resp);
            QJsonArray jmails = doc.object()["Body"].toObject()["SearchResponse"].toObject()["c"].toArray();

            QList<MailData> actual_mails;
            for (const QJsonValue &mail : jmails) {
                MailData m;
                QJsonArray jsenders = mail.toObject()["e"].toArray();
                for (const QJsonValue &sender : jsenders) {
                    Sender s;
                    s.email = sender.toObject()["a"].toString();
                    s.lastName = sender.toObject()["d"].toString();
                    s.fullName = sender.toObject()["p"].toString();
                    s.t = sender.toObject()["t"].toString();
                    m.senders.append(s);
                }
                QJsonArray jreceivers = mail.toObject()["m"].toArray();
                for (const QJsonValue &receiver : jreceivers) {
                    Receiver r;
                    r.id = receiver.toObject()["id"].toVariant().toLongLong();
                    r.l = receiver.toObject()["l"].toVariant().toLongLong();
                    r.timestamp = receiver.toObject()["d"].toVariant().toLongLong();
                    m.receivers.append(r);
                }
                m.subject = mail.toObject()["su"].toString();
                m.preview = mail.toObject()["fr"].toString();
                m.sf = mail.toObject()["sf"].toString();
                m.timestamp = mail.toObject()["d"].toVariant().toLongLong();
                m.id = mail.toObject()["id"].toVariant().toLongLong();
                m.u = mail.toObject()["u"].toVariant().toLongLong();
                m.n = mail.toObject()["n"].toVariant().toLongLong();
                actual_mails.append(m);
            }
            mails.append(actual_mails);

            emit mailsLoaded(actual_mails);
        }
        reply->deleteLater();
    });
}