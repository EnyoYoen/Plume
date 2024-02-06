#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Home : public QWidget
{
    Q_OBJECT

public:
    Home(QWidget *parent);

private:
    QHBoxLayout *lay = nullptr;
};