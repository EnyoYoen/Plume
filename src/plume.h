#pragma once

#include "widgets/bar/bottombar.h"
#include "widgets/home/home.h"
#include "widgets/calendar/calendar.h"
#include "widgets/mail/mail.h"

#include <QMap>
#include <QWidget>
#include <QVBoxLayout>
#include <QFontDatabase>

class Plume : public QWidget
{
    Q_OBJECT

    enum class Page : quint8 {
        Home,
        Calendar,
        Mail,
    };

public:
    Plume();

private:
    QMessageLogger *logger = nullptr;

    QVBoxLayout *lay = nullptr;
    BottomBar *bottombar = nullptr;
    Home *home = nullptr;
    Calendar *calendar = nullptr;
    Mail *mail = nullptr;

    Page page = Page::Home;
};