#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Mail : public QWidget
{
    Q_OBJECT

public:
    Mail(QWidget *parent);

private:
    QHBoxLayout *lay = nullptr;
};