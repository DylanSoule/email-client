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

    std::cout << "Enter app password\n";
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
        .address = "owenvanwestenburg@gmail.com"
    };
    EmailAddress sendAddr4 = {
        .name = "Lucca Townsend",
        .address = "luccatownsend@icloud.com"
    };
    EmailAddress sendAddr5 = {
        .name = "Brennon Hill",
        .address = "brennon.hill@gmail.com"
    };

    EmailMessage message = constructMessage(
        1,
        dylan,
        EmailMessage::State::sent,
        std::nullopt,
        std::vector<EmailAddress>{sendAddr3, sendAddr4},
        std::vector<EmailAddress>{sendAddr1},
        std::vector<EmailAddress>{sendAddr4},
        "Test message 1",
        "Do you know the unladen speed of a swallow"
    );

    CurlSmtpTransport transport(multiIface);

    SmtpClient client(dylan, transport);

    return 0;
}