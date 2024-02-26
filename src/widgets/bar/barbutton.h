#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>

#include "../common/button.h"

class BarButton : public Button
{
    Q_OBJECT
public:
    enum class Type : quint8 {
        Home,
        Calendar,
        Mail,
    };

    BarButton(Type type, QWidget *parent);

private:
    QLabel *content = nullptr;
};