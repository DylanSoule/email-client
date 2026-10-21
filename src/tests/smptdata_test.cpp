/***
 * THIS IS A TEMPORARY FILE TO RECORD HOW THIS WAS TESTED IT IS NOT AN ACTUAL TEST FILE
 *      WILL NEED TO BE UPDATED TO AN ACTUAL TEST FILE
 */

#include <iostream>

#include "email/EmailAccount.h"
#include "email/EmailAddress.h"

#include "transfer/SmtpClient.h"

#include "network/CurlSmtpTransport.h"

void printmsg(SmtpData& mess) {
    std::cout << mess.username << "\n";
    std::cout << mess.password << "\n";
    std::cout << mess.serverUrl << "\n";

    std::cout << mess.sender << "\n";

    for (const auto& person : mess.recipients) {
        std::cout << "Recipient: " << person << "\n";
    }

    for (const auto& person : mess.carbonCopy) {
        std::cout << "CC: " << person << "\n";
    }

    for (const auto& person : mess.blindCarbonCopy) {
        std::cout << "BCC: " << person << "\n";
    }

    std::cout << mess.subject << "\n";
    std::cout << mess.contents << "\n";
}

int main() {
    EmailAccount dylan = {
        .id = "1", 
        .emailAddress = "dylan.soule@icloud.com",
        .name = "Dylan Soule",
        .username = "dylan.soule@icloud.com",
        .password = "xxxx-xxxx-xxxx-xxxx",
        .loginMethod = EmailAccount::LoginMethod::simple,
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

    CurlSmtpTransport transport_obj;

    SmtpClient client(dylan, transport_obj);

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
        .recipients = {receiver, receiver2},
        .blindCarbonCopy = {bcc},
        .subject = "Subject line",
        .contents = "Hello person I am here"
    };

    SmtpData mess1 = client.buildMessage(message1);
    SmtpData mess2 = client.buildMessage(message2);

    printmsg(mess1);
    printmsg(mess2);

    return 0;
}