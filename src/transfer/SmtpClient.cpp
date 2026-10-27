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


std::string SmtpClient::receiverList(const std::vector<EmailAddress>& tempMessage) {
    bool first = true;
    std::string result;
    for (const EmailAddress& recipient : tempMessage) {
        if (!first) {
            result += ", ";
            first = false;
        }
        result += recipient.name + " <" + recipient.address + ">";
    }
    return result;
}

SmtpData SmtpClient::buildMessage(const EmailMessage& message) {
    SmtpData temp;

    temp.username = account_.username;
    temp.password = account_.password;

    temp.serverUrl = account_.smtpServer + ":" + std::to_string(account_.smtpPort);

    temp.date = getRfc5322Date();

    temp.sender = account_.name.empty() ? 
        "<" + account_.emailAddress + ">" :
        account_.name + "<" + account_.emailAddress + ">";
    
    if (message.recipients.empty() && message.carbonCopy.empty() && message.blindCarbonCopy.empty()) {
        throw std::invalid_argument("No recipients");
    }

    temp.recipients = receiverList(message.recipients);
    temp.carbonCopy = receiverList(message.carbonCopy);
    temp.blindCarbonCopy = receiverList(message.blindCarbonCopy);

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