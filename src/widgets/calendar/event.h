#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDateTime>
#include <QMouseEvent>
#include <QEnterEvent>

#include "libical/ical.h"

class Event : public QWidget
{
    Q_OBJECT

public:
    Event(QDateTime datetime, icalcomponent *comp, QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;

    QVBoxLayout *lay = nullptr;
    bool pressed = false;
};