#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

class Calendar : public QWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent);

private:
    const char* days[8] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    QVBoxLayout *lay = nullptr;

    QScrollArea *scroll = nullptr;
};