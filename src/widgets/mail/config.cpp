#include "config.h"

#include <QStandardPaths>
#include <QFileInfo>

void MailConfig::load()
{
    checkDataFolder();

    QFile configFile(mailFolderPath + "config.txt");
    if (configFile.open(QFile::ReadOnly)) {
        QString config = configFile.readAll();
        QStringList configLines = config.split('\n');
        for (QString line : configLines) {
            if (line.startsWith("login")) {
                login = line.split(':')[1];
            } else if (line.startsWith("password")) {
                password = line.split(':')[1];
            } else if (line.startsWith("mail")) {
                mail = line.split(':')[1];
            }
        }
    }
}

bool MailConfig::add(QString name, QVariant value)
{
    checkDataFolder();

    bool found = false;
    QStringList configLines;
    QString configLine = name + ":" + value.toString();
    QFile configFile(mailFolderPath + "config.txt");
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


void MailConfig::checkDataFolder()
{
    if (dataFolderPath.isNull()) {
        dataFolderPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        dataFolder = QDir(dataFolderPath);
        mailFolderPath = dataFolderPath + "/mail/";

        QStringList splits = dataFolderPath.split('/');
        splits.removeLast();
        appdataPath = splits.join('/');
    }

    
    if (!dataFolder.exists())
        QDir(appdataPath).mkdir("Plume");

    if (!QDir(mailFolderPath).exists())
        dataFolder.mkdir("mail");
    
    if (!QFileInfo(mailFolderPath + "config.txt").isFile())
        QFile(mailFolderPath + "config.txt").open(QFile::WriteOnly);
}