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

    std::string appPsswd;
    std::getline(std::cin, appPsswd);

    EmailAccount dylan = {
        .id = "1",
        .emailAddress = "dylan.soule@icloud.com",
        .name = "Dylan Soule",
        .username = "dylan.soule@icloud.com",
        .password = appPsswd,
        .loginMethod = EmailAccount::LoginMethod::simple,
        .smtpServer = "smtp.mail.me.com",
        .smtpPort = 587
    };

    EmailAddress sendAddr1 = {
        .name = "Dylan Soule",
        .address = "dylan.soule@icloud.com"
    };
    EmailAddress sendAddr2 = {
        .name = "Dylan Soule",
        .address = "2141247@gmail.com"
    };
    EmailAddress sendAddr3 = {
        .name = "Owen Van Westenburg",
        .address = "2140413@jeffcoschools.us"
    };
    EmailAddress sendAddr4 = {
        .name = "Lucca Townsend",
        .address = "2132410@jeffcoschools.us"
    };
    EmailAddress sendAddr5 = {
        .name = "Brennon Hill",
        .address = "2003081@jeffcoschools.us"
    };

    EmailMessage message = constructMessage(
        1,
        dylan,
        EmailMessage::State::sent,
        std::nullopt,
        std::vector<EmailAddress>{sendAddr2, sendAddr3},
        std::vector<EmailAddress>{sendAddr1},
        std::vector<EmailAddress>{sendAddr4, sendAddr5},
        "Test message 1",
        "Do you know the unladen speed of a swallow"
    );

    EmailMessage message2 = constructMessage(
        2,
        dylan,
        EmailMessage::State::sent,
        message.messageId,
        std::vector<EmailAddress>{sendAddr2, sendAddr3},
        std::vector<EmailAddress>{sendAddr1},
        std::vector<EmailAddress>{sendAddr4, sendAddr5},
        "RE Test message 1",
        "African or American Swallow"
    );

    CurlSmtpTransport transport;

    SmtpClient client(dylan, transport);

    SmtpData mess1 = client.buildMessage(message);
    client.send(mess1);

    return 0;
}