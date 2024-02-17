#include "eventpage.h"

#include <QPushButton>
#include <QScrollArea>

QWidget *buildInfoWidget(QPixmap icon, QWidget *info, QWidget *parent)
{
    QWidget *infoCont = new QWidget(parent);
    QHBoxLayout *infoLay = new QHBoxLayout(infoCont);
    QLabel *iconWidget = new QLabel(infoCont);
    if (!icon.isNull())
        iconWidget->setPixmap(icon);
    infoLay->addWidget(iconWidget);
    infoLay->addWidget(info);
    infoLay->setSpacing(20);
    infoLay->setContentsMargins(0, 0, 0, 0);
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
    QPushButton *backButton = new QPushButton(header);
    QObject::connect(backButton, &QPushButton::clicked, [this](){ emit closed(); });
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
    headerLay->addWidget(backButton);
    headerLay->addWidget(title);
    lay->addWidget(header);
        

    QScrollArea *scroll = new QScrollArea(this);
    QWidget *scrollContent = new QWidget(scroll);
    QVBoxLayout *scrollLay = new QVBoxLayout(scrollContent);
    
    QLabel *infoTitle = new QLabel("Informations", scrollContent);
    scrollLay->addWidget(infoTitle);

    QWidget *datetime = new QWidget(scrollContent);
    QVBoxLayout *datetimeLay = new QVBoxLayout(datetime);
    QLabel *date = new QLabel(dt.date().toString(), datetime);
    QTime end = dt.time().addSecs(duration);
    QLabel *time = new QLabel(QString::number(dt.time().hour()) + ":"
        + QString::number(dt.time().minute())
        + " - " + QString::number(end.hour())
        + ":" + QString::number(end.minute()), datetime);
    datetimeLay->addWidget(date);
    datetimeLay->addWidget(time);
    QWidget *datetimeInfo = buildInfoWidget(QPixmap(), datetime, scrollContent);
    scrollLay->addWidget(datetimeInfo);

    QString desc = QString(icalcomponent_get_description(comp));
    if (ade) {
        QStringList splits = desc.split('\n');
        if (splits.length() > 0) {
            QStringList intro = splits[1].split(']');
            if (intro.length() > 0) {
                QLabel *classM = new QLabel(intro[1].trimmed(), scrollContent); 
                QWidget *classInfo = buildInfoWidget(QPixmap(), classM, scrollContent);
                scrollLay->addWidget(classInfo);
            }

            const char *location = icalcomponent_get_location(comp);
            if (!location) {
                QLabel *classroom = new QLabel(QString("Room ") + icalcomponent_get_location(comp), scrollContent);
                QWidget *classroomInfo = buildInfoWidget(QPixmap(), classroom, scrollContent);
                scrollLay->addWidget(classroomInfo);
            }

            QLabel *teacher = new QLabel(splits[5], scrollContent);
            QWidget *teacherInfo = buildInfoWidget(QPixmap(), teacher, scrollContent);
            scrollLay->addWidget(teacherInfo);

            QLabel *group = new QLabel(QString("Group ") + splits[4], scrollContent);
            QWidget *groupInfo = buildInfoWidget(QPixmap(), group, scrollContent);
            scrollLay->addWidget(groupInfo);
        }
    }

    QLabel *descTitle = new QLabel("Description", scrollContent);
    scrollLay->addWidget(descTitle);
    QLabel *descContent = new QLabel(desc.replace('\n', ' ').trimmed(), scrollContent);
    scrollLay->addWidget(descContent);

    scroll->setWidget(scrollContent);
    lay->addWidget(scroll);


    setProperty("class", "calendar-event-page");
    setAttribute(Qt::WA_StyledBackground);
}