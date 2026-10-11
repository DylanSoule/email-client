#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "EmailAccount.h"
#include "EmailAddress.h"

/*
Defines an email message on the email client
This message is of any type, received, draft, or already sent

Plaintext message to be shown to the user
    Mime parsing and compilation will happen before assignment to message struct
*/
struct EmailMessage {
    int id;

    EmailAddress sender;
    
    std::vector<EmailAddress> recipients;
    std::vector<EmailAddress> carbonCopy;
    std::vector<EmailAddress> blindCarbonCopy;

    std::string subject;
    std::string contents;

    enum class type { received, draft, sent };

    std::chrono::system_clock::time_point time;
};