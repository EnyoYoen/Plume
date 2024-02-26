#pragma once

#include "barbutton.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class BottomBar : public QWidget
{
    Q_OBJECT

public:
    BottomBar(QWidget *parent);

signals:
    void homeButtonPressed();
    void calendarButtonPressed();
    void mailButtonPressed();

private:
    QHBoxLayout *lay = nullptr;
    BarButton *homeButton = nullptr;
    BarButton *calendarButton = nullptr;
    BarButton *mailButton = nullptr;
};