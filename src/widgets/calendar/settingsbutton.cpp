#include "settingsbutton.h"

#include <QHBoxLayout>

SettingsButton::SettingsButton(QList<QWidget *> content, qint32 stretchIndex, qint32 spacing, QWidget *parent)
    : Button(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setSpacing(spacing);
    lay->setContentsMargins(5, 5, 5, 5);

    for (quint16 i = 0 ; i < content.size() ; i++) {
        if (i == stretchIndex)
            lay->addStretch();
        lay->addWidget(content[i]);
        content[i]->setParent(this);
    }

    if (stretchIndex >= content.size())
        lay->addStretch();
    
    setFixedHeight(35);
    setProperty("class", property("class").toString() + " calendar-settigns-button");
}