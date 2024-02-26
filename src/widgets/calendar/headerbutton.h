#pragma once

#include "../common/button.h"

class HeaderButton : public Button
{
    Q_OBJECT
public:
    enum class Type : quint8 {
        Left,
        Right,
        Today,
        Settings
    };

    HeaderButton(Type type, QWidget *parent);
};