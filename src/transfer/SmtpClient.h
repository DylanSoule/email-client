#pragma once

#include <string>
#include <vector>

#include "../email/EmailAccount.h"
#include "../email/EmailMessage.h"
#include "../email/EmailAddress.h"
#include "../network/SmtpTransport.h"

/*
Sets up data to be sent over smtp with all required parts
    sender account, recipients(normal, cc, bcc), subject, and contents
*/
struct SmtpData {
    std::string username;
    std::string password;

    std::string serverUrl;

    std::string sender;
    std::vector<EmailAddress> recipients;
    std::vector<EmailAddress> carbonCopy;
    std::vector<EmailAddress> blindCarbonCopy;

    std::string subject;
    std::string contents;
};



class SmtpClient {
    public:
        SmtpClient(
            const EmailAccount& account,
            SmtpTransport& transport
        );

        SmtpResult send(EmailMessage& message);
    private:
        const EmailAccount& account_;
        SmtpTransport& transport_;
};