#include "eventpage.h"

#include <QScrollArea>
#include <QScrollBar>

#include <QTimeZone>

#include "backbutton.h"

QWidget *buildInfoWidget(QPixmap icon, QWidget *info, QWidget *parent)
{
    QWidget *infoCont = new QWidget(parent);
    QHBoxLayout *infoLay = new QHBoxLayout(infoCont);
    QLabel *iconWidget = new QLabel(infoCont);
    if (!icon.isNull()) {
        iconWidget->setPixmap(icon.scaledToWidth(40, Qt::TransformationMode::SmoothTransformation));
        iconWidget->setFixedSize(40, 40);
        iconWidget->setProperty("class", "calendar-event-page-info-icon");
    }
    infoLay->addWidget(iconWidget);
    infoLay->addWidget(info);
    infoLay->setSpacing(20);
    infoLay->setContentsMargins(0, 0, 0, 0);
    infoCont->setProperty("class", "calendar-event-page-info-container");
    return infoCont;
} 

EventPage::EventPage(icalcomponent *comp, QDateTime dt, qint32 duration, QWidget *parent)
    : QWidget(parent)
{
    lay = new QVBoxLayout(this);
    
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    bool ade = QString(icalcomponent_get_uid(comp)).startsWith("ADE");

    QWidget *header = new QWidget(this);
    QVBoxLayout *headerLay = new QVBoxLayout(header);
    BackButton *backButton = new BackButton(header);
    QObject::connect(backButton, &BackButton::clicked, [this](){ emit closed(); });
    QLabel *title;
    QString summary = icalcomponent_get_summary(comp);
    if (ade) {
        QStringList splits = summary.split(':');
        splits.removeAll("");
        if (splits.length() > 4) 
            title = new QLabel(splits[4], header);
        else
            title = new QLabel(summary, header);

    } else {
        title = new QLabel(summary, header);
    }
    header->setProperty("class", "calendar-page-header");
    title->setProperty("class", "calendar-page-title");
    headerLay->addWidget(backButton);
    headerLay->addWidget(title);
    headerLay->setContentsMargins(10, 10, 10, 20);
    headerLay->setSpacing(20);
    lay->addWidget(header);
        

    QScrollArea *scroll = new QScrollArea(this);
    QWidget *scrollContent = new QWidget(scroll);
    QVBoxLayout *scrollLay = new QVBoxLayout(scrollContent);
    
    QLabel *infoTitle = new QLabel("Informations", scrollContent);
    QWidget *line1 = new QWidget(scrollContent);
    line1->setFixedSize(170, 10);
    line1->setProperty("class", "calendar-page-line");
    line1->setAttribute(Qt::WA_StyledBackground, true);
    infoTitle->setProperty("class", "calendar-page-subtitle");
    scrollLay->addSpacing(10);
    scrollLay->addWidget(infoTitle);
    scrollLay->addSpacing(4);
    scrollLay->addWidget(line1);
    scrollLay->addSpacing(8);

    QWidget *datetime = new QWidget(scrollContent);
    QVBoxLayout *datetimeLay = new QVBoxLayout(datetime);
    QLabel *date = new QLabel(dt.date().toString(), datetime);
    date->setProperty("class", "calendar-event-page-info");
    dt.setTimeZone(QTimeZone("Etc/UTC"));
    dt.toTimeZone(QDateTime::currentDateTime().timeZone());
    QTime end = dt.time().addSecs(duration);
    QLabel *time = new QLabel(QString::number(dt.time().hour()) + ":"
        + (dt.time().minute() < 10 ? "0" : "") + QString::number(dt.time().minute())
        + " - " + QString::number(end.hour())
        + ":" + (end.minute() < 10 ? "0" : "") + QString::number(end.minute()), datetime);
    time->setProperty("class", "calendar-event-page-info-time");
    datetime->setProperty("class", "calendar-event-page-info");
    datetimeLay->setContentsMargins(0, 0, 0, 0);
    datetimeLay->setSpacing(0);
    datetimeLay->addWidget(date);
    datetimeLay->addWidget(time);
    datetime->setProperty("class", "calendar-event-page-info");
    QWidget *datetimeInfo = buildInfoWidget(QPixmap(QString(":/clock.png")), datetime, scrollContent);
    scrollLay->addWidget(datetimeInfo);

    QString desc = QString(icalcomponent_get_description(comp));
    if (ade) {
        QStringList splits = desc.split('\n');
        if (splits.length() > 0) {
            QStringList intro = splits[1].split(']');
            if (intro.length() > 1 && !QString(intro[1]).trimmed().isEmpty()) {
                QLabel *classM = new QLabel(intro[1].trimmed(), scrollContent); 
                classM->setProperty("class", "calendar-event-page-info");
                QWidget *classInfo = buildInfoWidget(QPixmap(QString(":/list.png")), classM, scrollContent);
                scrollLay->addWidget(classInfo);
            }

            const char *location = icalcomponent_get_location(comp);
            if (!location && !QString(location).isEmpty()) {
                QLabel *classroom = new QLabel(QString("Room ") + location, scrollContent);
                classroom->setProperty("class", "calendar-event-page-info");
                QWidget *classroomInfo = buildInfoWidget(QPixmap(QString(":/pin.png")), classroom, scrollContent);
                scrollLay->addWidget(classroomInfo);
            }

            if (splits.length() > 5 && !QString(splits[5]).isEmpty()) {
                QLabel *teacher = new QLabel(splits[5], scrollContent);
                teacher->setProperty("class", "calendar-event-page-info");
                QWidget *teacherInfo = buildInfoWidget(QPixmap(QString(":/graduation.png")), teacher, scrollContent);
                scrollLay->addWidget(teacherInfo);
            }

            if (splits.length() > 4 && !QString(splits[4]).isEmpty()) {
                QLabel *group = new QLabel(QString("Group ") + splits[4], scrollContent);
                group->setProperty("class", "calendar-event-page-info");
                QWidget *groupInfo = buildInfoWidget(QPixmap(QString(":/group.png")), group, scrollContent);
                scrollLay->addWidget(groupInfo);
            }
        }
    }

    scrollLay->addSpacing(50);
    QLabel *descTitle = new QLabel("Description", scrollContent);
    descTitle->setProperty("class", "calendar-page-subtitle");
    QWidget *line2 = new QWidget(scrollContent);
    line2->setFixedSize(170, 10);
    line2->setProperty("class", "calendar-page-line");
    line2->setAttribute(Qt::WA_StyledBackground, true);
    scrollLay->addWidget(descTitle);
    scrollLay->addSpacing(4);
    scrollLay->addWidget(line2);
    scrollLay->addSpacing(8);
    QLabel *descContent = new QLabel(desc.replace('\n', ' ').trimmed(), scrollContent);
    descContent->setWordWrap(true);
    descContent->setProperty("class", "calendar-event-page-description-content");
    scrollLay->addWidget(descContent);
    scrollLay->setContentsMargins(5, 5, 5, 5);
    scrollLay->setSpacing(0);
    scrollLay->addStretch();

    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setAttribute(Qt::WA_StyledBackground, true);
    scroll->setProperty("class", "calendar-page-scroll");
    lay->addWidget(scroll);


    setProperty("class", "calendar-page");
    setAttribute(Qt::WA_StyledBackground);
}