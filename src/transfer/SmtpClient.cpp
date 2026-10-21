#include <vector>
#include <string>
#include <stdexcept>

#include "SmtpClient.h"

#include "../email/EmailAccount.h"
#include "../email/EmailMessage.h"
#include "../email/EmailAddress.h"
#include "../network/SmtpTransport.h"

SmtpClient::SmtpClient(
            const EmailAccount& account,
            SmtpTransport& transport
        ) : account_(account), transport_(transport) {}


SmtpData SmtpClient::buildMessage(const EmailMessage& message) {
    SmtpData temp;

    temp.username = account_.username;
    temp.password = account_.password;

    temp.serverUrl = account_.smtpServer + ":" + std::to_string(account_.smtpPort);

    temp.sender = account_.name.empty() ? 
        "<" + account_.emailAddress + ">" :
        account_.name + "<" + account_.emailAddress + ">";
    
    if (message.recipients.empty() && message.carbonCopy.empty() && message.blindCarbonCopy.empty()) {
        throw std::invalid_argument("No recipients");
    }

    for (const EmailAddress& recipient : message.recipients) {
        temp.recipients.push_back(
            recipient.name.empty() ?
                "<" + recipient.address + ">" :
                recipient.name + "<" + recipient.address + ">"
        );
    }
    for (const EmailAddress& recipient : message.carbonCopy) {
        temp.carbonCopy.push_back(
            recipient.name.empty() ?
                "<" + recipient.address + ">" :
                recipient.name + "<" + recipient.address + ">"
        );
    }
    for (const EmailAddress& recipient : message.blindCarbonCopy) {
        temp.blindCarbonCopy.push_back(
            recipient.name.empty() ?
                "<" + recipient.address + ">" :
                recipient.name + "<" + recipient.address + ">"
        );
    }

    temp.subject = message.subject;
    temp.contents = message.contents;

    return temp;
}

SmtpResult SmtpClient::send(SmtpData& data) {
    SmtpResult res = transport_.send(data);
    if (!res.success) {
        throw std::runtime_error("Failed to send data");
    }

    return res;
}