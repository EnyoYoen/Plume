#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>

class Button : public QLabel
{
    Q_OBJECT
public:
    Button(QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    
    bool pressed = false;
};