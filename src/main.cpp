#include <iostream>

#include "email/EmailAccount.h"
#include "email/EmailAddress.h"
#include "email/EmailMessage.h"

#include "transfer/SmtpClient.h"

#include "network/CurlSmtpTransport.h"

int main () {
    EmailAccount dylan = {
        .id = "1", 
        .emailAddress = "dylan.soule@icloud.com",
        .name = "Dylan Soule",
        .username = "dylan.soule@icloud.com",
        .password = "xxxx-xxxx-xxxx-xxxx",
        .loginMethod = EmailAccount::loginMethod::simple,
        .smtpServer = "smtp.mail.me.com",
        .smtpPort = 443
    };

    EmailAddress receiver = {
        .name = "John Smith",
        .address = "test.test@gmail.com"
    };

    EmailAddress cc = {
        .address = "jane.smith@gmail.com"  
    };

    EmailAddress receiver2 = {
        .name = "Kason Nick",
        .address = "kason.nick@proton.me"
    };

    EmailAddress bcc = {
        .address = "he.man@hotmail.com"
    };

    CurlSmtpTransport transport;

    SmtpClient client(dylan, transport);

    EmailMessage message1 = {
        .id = 1,
        .sender = dylan,
        .recipients = {receiver},
        .carbonCopy = {cc},
        .blindCarbonCopy = {bcc},
        .subject = "",
        .contents = "Hello person I am here"
    };

    EmailMessage message2 = {
        .id = 2,
        .sender = dylan,
        .recipients = {receiver,receiver2},
        .blindCarbonCopy = {bcc},
        .subject = "Subject line",
        .contents = "Hello person I am here"
    };

    SmtpData mess1 = client.buildMessage(message1);

    SmtpData mess2 = client.buildMessage(message2);

    std::cout << mess1;
    std::cout <<
}