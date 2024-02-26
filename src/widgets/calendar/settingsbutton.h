#pragma once

#include "../common/button.h"

class SettingsButton : public Button
{
    Q_OBJECT
public:
    SettingsButton(QList<QWidget *> content, qint32 stretchIndex, qint32 spacing, QWidget *parent);
};