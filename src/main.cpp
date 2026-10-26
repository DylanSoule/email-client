#include <iostream>

#include "email/EmailAccount.h"
#include "email/EmailAddress.h"
#include "email/EmailMessage.h"

#include "transfer/SmtpClient.h"

#include "network/CurlGlobal.h"
#include "network/CurlMulti.h"
#include "network/CurlSmtpTransport.h"

int main() {
    CurlGlobal globalIface;
    CurlMulti multiIface;

    EmailAccount dylan = {
        .id = "1",
        .emailAddress = "dylan.soule@icloud.com",
        .name = "Dylan Soule",
        .username = "dylan.soule@icloud.com",
        .password = "xxxx-xxxx-xxxx-xxxx",
        .loginMethod = EmailAccount::LoginMethod::simple,
        .smtpServer = "smtp.mail.me.com",
        .smtpPort = 587
    };

    EmailAddress sendAddr1 = {

    };
    EmailAddress sendAddr2 = {

    };

    EmailMessage message = {

    };

    return 0;
}