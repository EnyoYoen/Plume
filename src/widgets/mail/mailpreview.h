#pragma once

#include <QWidget>

#include "../common/button.h"
#include "api.h"

class MailPreview : public Button
{
public:
    MailPreview(Api::MailData& data, QWidget *parent);
};