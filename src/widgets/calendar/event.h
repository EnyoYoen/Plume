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

    void reduce(qint8 count = 1);

    qint8 order = 0;
    qint8 reduceFactor = 1;
    icalcomponent *c;

signals:
    void clicked();

private:
    void showEvent(QShowEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;

    QVBoxLayout *lay = nullptr;
    bool reduced = false;
    bool pressed = false;
};