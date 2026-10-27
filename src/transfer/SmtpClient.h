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

    std::string date;
    std::string sender;
    std::string recipients;
    std::string carbonCopy;
    std::string blindCarbonCopy;

    std::string messageId;

    std::string subject;
    std::string contents;
};


/*
Configures message to be sent via SMTP
*/
class SmtpClient {
    public:
        /*
        Passes class params to private class variables
        */
        SmtpClient(
            const EmailAccount& account,
            SmtpTransport& transport
        );
        
        /*
        Builds SmtpData object based off of class variables account_, transport_, and passed message struct
            Params: message - pointer to EmailMessage struct
            Result: SmtpData struct - configured to be sent to transport class
        */
        SmtpData buildMessage(const EmailMessage& message); 

        /*
        Formats receiver vector into usable string for SMTP
        */
        std::string receiverList(const std::vector<EmailAddress>& tempMessage);

        /*
        Sends data to transport class to be sent over SMTP
        */
        SmtpResult send(SmtpData& data);
    private:
        const EmailAccount& account_;
        SmtpTransport& transport_;
};