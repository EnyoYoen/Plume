#include "settings.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>

#include "settingsbutton.h"
#include "enablebutton.h"

// TODO : use a QHash to know if the calendar is enabled for each calendar name
Settings::Settings(QHash<CalendarName, bool> pcalendarEnabled, QWidget *parent)
    : calendarsEnabled(pcalendarEnabled), QWidget(parent)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addStretch();

    QWidget *sub = new QWidget(this);
    QHBoxLayout *subLay = new QHBoxLayout(sub);
    subLay->setSpacing(0);
    subLay->setContentsMargins(0, 0, 0, 0);
    subLay->addStretch();

    innerContent = new QWidget(sub);
    innerContent->setProperty("class", "calendar-settings-content");
    QVBoxLayout *innerLay = new QVBoxLayout(innerContent);
    innerLay->setSpacing(0);
    innerLay->setContentsMargins(10, 10, 10, 10);

    QList<QWidget *> dayContent;
    dayContent.append(new QLabel("1 day"));
    dayContent[0]->setProperty("class", "calendar-settings-button-text");
    SettingsButton *day = new SettingsButton(dayContent, 1, 0, innerContent);
    QObject::connect(day, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(0); });

    QList<QWidget *> daysContent;
    daysContent.append(new QLabel("5 days"));
    daysContent[0]->setProperty("class", "calendar-settings-button-text");
    SettingsButton *days = new SettingsButton(daysContent, 1, 0, innerContent);
    QObject::connect(days, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(1); });

    QList<QWidget *> weekContent;
    weekContent.append(new QLabel("Week"));
    weekContent[0]->setProperty("class", "calendar-settings-button-text");
    SettingsButton *week = new SettingsButton(weekContent, 1, 0, innerContent);
    QObject::connect(week, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(2); });

    QList<QWidget *> monthContent;
    monthContent.append(new QLabel("Month"));
    monthContent[0]->setProperty("class", "calendar-settings-button-text");
    SettingsButton *month = new SettingsButton(monthContent, 1, 0, innerContent);
    QObject::connect(month, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(3); });


    QList<QWidget *> calendarsContent;
    QLabel *calendarIcon = new QLabel();
    calendarIcon->setPixmap(QPixmap(":/calendar.png").scaledToHeight(30, Qt::TransformationMode::SmoothTransformation));
    calendarsContent.append(calendarIcon);
    calendarsContent.append(new QLabel("Calendars"));
    calendarsContent[1]->setProperty("class", "calendar-settings-button-text");
    SettingsButton *calendars = new SettingsButton(calendarsContent, 2, 15, innerContent);
    QObject::connect(calendars, &SettingsButton::clicked, [this](){ emit calendarsClicked(); });


    innerLay->addWidget(day);
    innerLay->addWidget(days);
    innerLay->addWidget(week);
    innerLay->addWidget(month);
    innerLay->addWidget(calendars);


    for (CalendarName name : calendarsEnabled.keys()) {
        QList<QWidget *> calendarContent;
        QString nameStr = (name.first.isNull() ? name.second.fileName() : name.first);
        calendarContent.append(new QLabel(nameStr));
        calendarContent[0]->setProperty("class", "calendar-settings-button-text");
        EnableButton *button = new EnableButton(calendarsEnabled[name], nullptr);
        calendarContent.append(button);
        SettingsButton *calendar = new SettingsButton(calendarContent, 1, 20, innerContent);
        innerLay->addWidget(calendar);

        QObject::connect(calendar, &SettingsButton::clicked, [this, button, name]() {
            bool enabled = calendarsEnabled[name];
            calendarsEnabled[name] = !enabled;
            button->setEnable(!enabled);
            emit calendarClicked(name, !enabled);
        });
    }
    innerLay->addStretch();

    subLay->addWidget(innerContent);
    subLay->addStretch();

    lay->addWidget(sub);
    lay->addStretch();

    sub->setAttribute(Qt::WA_StyledBackground, true);
    sub->setProperty("class", "calendar-settings-sub");

    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setProperty("class", "calendar-settings");
}

void Settings::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !innerContent->rect().contains(event->position().x(), event->position().y())) {
        emit closed();
    }
}