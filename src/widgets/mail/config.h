#pragma once

#include <QString>
#include <QDir>

// TODO: Unify with calendar config
class MailConfig {

public:
    MailConfig() = default;

    void load();
    bool add(QString name, QVariant value);

    void checkDataFolder();

    QString login;
    QString password;
    QString mail;

private:
    QDir dataFolder;

    QString appdataPath;
    QString dataFolderPath;
    QString mailFolderPath;
};
