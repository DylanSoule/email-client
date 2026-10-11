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
struct message {
    int id;

    emailAddress sender;
    
    std::vector<emailAddress> recipients;
    std::vector<emailAddress> carbonCopy;
    std::vector<emailAddress> blindCarbonCopy;

    std::string subject;
    std::string contents;

    enum class type { received, draft, sent };

    std::chrono::system_clock::time_point time;
};