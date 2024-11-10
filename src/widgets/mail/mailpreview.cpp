#include "mailpreview.h"

#include <QVBoxLayout>

MailPreview::MailPreview(Api::MailData& data, QWidget *parent)
    : Button(parent)
{
    setProperty("class", "mail-preview");

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(10, 10, 10, 10);
    lay->setSpacing(0);

    QLabel *sender = new QLabel(data.senders[0].fullName, this);
    QLabel *subject = new QLabel(data.subject, this);
    QLabel *preview = new QLabel(data.preview, this);

    sender->setProperty("class", "mail-preview-sender");
    subject->setProperty("class", "mail-preview-subject");
    preview->setProperty("class", "mail-preview-preview");

    lay->addWidget(sender);
    lay->addWidget(subject);
    lay->addWidget(preview);

    this->setFixedHeight(100);
}