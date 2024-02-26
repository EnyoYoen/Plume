#pragma once

#include <QLabel>

class EnableButton : public QLabel
{
    Q_OBJECT
public:
    EnableButton(bool enable, QWidget *parent);

    void setEnable(bool enable);

private:
    bool enabled;
};