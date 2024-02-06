#include "plume.h"

#include <QDirIterator>
#include <QLibrary>
#include <QFunctionPointer>
#include <QMessageLogger>

Plume::Plume()
    : QWidget()
{
    logger = new QMessageLogger();

    if (QFontDatabase::addApplicationFont(":/whitney.otf")) {
        logger->warning("Can't add application font : whitney");
    }

    QFile style(":/plume-style.css");
    style.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(style.readAll());
    setStyleSheet(styleSheet);

    lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    home = new Home(this);
    calendar = new Calendar(this);
    calendar->hide();
    mail = new Mail(this);
    mail->hide();
    BottomBar *bottombar = new BottomBar(this);

    QObject::connect(bottombar, &BottomBar::homeButtonPressed, [this]() {
        if (page != Page::Home) {
            QLayoutItem *item = lay->itemAt(0);
            item->widget()->hide();
            lay->removeItem(item);
            lay->insertWidget(0, home);
            home->show();
            page = Page::Home;
        }
    });
    QObject::connect(bottombar, &BottomBar::calendarButtonPressed, [this]() {
        if (page != Page::Calendar) {
            QLayoutItem *item = lay->itemAt(0);
            item->widget()->hide();
            lay->removeItem(item);
            lay->insertWidget(0, calendar);
            calendar->show();
            page = Page::Calendar;
        }
    });
    QObject::connect(bottombar, &BottomBar::mailButtonPressed, [this]() {
        if (page != Page::Mail) {
            QLayoutItem *item = lay->itemAt(0);
            item->widget()->hide();
            lay->removeItem(item);
            lay->insertWidget(0, mail);
            mail->show();
            page = Page::Mail;
        }
    });

    lay->addWidget(home);
    lay->addWidget(bottombar);

    setProperty("class", "plume");

    setMinimumSize(1000, 800); // TODO : temporary
}