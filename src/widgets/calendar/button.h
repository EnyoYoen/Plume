#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>

class HeaderButton : public QLabel
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

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;

    QLabel *content = nullptr;
    bool pressed = false;
};