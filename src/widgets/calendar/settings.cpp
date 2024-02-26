#include "settings.h"

#include <QVBoxLayout>

#include "settingsbutton.h"
#include "enablebutton.h"

Settings::Settings(QList<CalendarName> calendarsNames, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);


    innerContent = new QWidget(this);
    innerContent->setProperty("class", "calendar-settings-content");
    QVBoxLayout *innerLay = new QVBoxLayout(innerContent);
    innerLay->setSpacing(0);
    innerLay->setContentsMargins(0, 0, 0, 0);

    QList<QWidget *> dayContent;
    dayContent.append(new QLabel("1 day"));
    SettingsButton *day = new SettingsButton(dayContent, 1, 0, innerContent);
    QObject::connect(day, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(0); });

    QList<QWidget *> daysContent;
    daysContent.append(new QLabel("5 days"));
    SettingsButton *days = new SettingsButton(daysContent, 1, 0, innerContent);
    QObject::connect(days, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(1); });

    QList<QWidget *> weekContent;
    weekContent.append(new QLabel("Week"));
    SettingsButton *week = new SettingsButton(weekContent, 1, 0, innerContent);
    QObject::connect(week, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(2); });

    QList<QWidget *> monthContent;
    monthContent.append(new QLabel("Month"));
    SettingsButton *month = new SettingsButton(monthContent, 1, 0, innerContent);
    QObject::connect(month, &SettingsButton::clicked, [this](){ emit calendarTypeClicked(3); });


    QList<QWidget *> calendarsContent;
    QLabel *calendarIcon = new QLabel();
    calendarIcon->setPixmap(QPixmap(":/calendar.png").scaledToHeight(30, Qt::TransformationMode::SmoothTransformation));
    calendarsContent.append(calendarIcon);
    calendarsContent.append(new QLabel("Calendars"));
    SettingsButton *calendars = new SettingsButton(calendarsContent, 2, 15, innerContent);
    QObject::connect(calendars, &SettingsButton::clicked, [this](){ emit calendarsClicked(); });


    innerLay->addWidget(day);
    innerLay->addWidget(days);
    innerLay->addWidget(week);
    innerLay->addWidget(month);
    innerLay->addWidget(calendars);


    for (qsizetype i = 0 ; i < calendarsNames.size() ; i++) {
        QList<QWidget *> calendarContent;
        QString name = (calendarsNames[i].first.isNull() ? calendarsNames[i].second.fileName() : calendarsNames[i].first);
        calendarContent.append(new QLabel(name));
        EnableButton *button = new EnableButton(true, nullptr);
        calendarContent.append(button);
        SettingsButton *calendar = new SettingsButton(calendarContent, 1, 0, innerContent);
        enable[calendarsNames[i]] = true;
        innerLay->addWidget(calendar);

        QObject::connect(calendar, &SettingsButton::clicked, [this, button, i, calendarsNames]() {
            bool enabled = enable[calendarsNames[i]];
            enable[calendarsNames[i]] = !enabled;
            button->setEnable(!enabled);
            emit calendarClicked(calendarsNames[i], !enabled);
        });
    }

    lay->addWidget(innerContent);
    lay->addStretch();

    this->setProperty("class", "calendar-settings");
}

void Settings::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !innerContent->rect().contains(event->position().x(), event->position().y())) {
        emit closed();
    }
}