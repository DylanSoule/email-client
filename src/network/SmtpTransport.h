#pragma once

#include <string>

#include "../email/EmailAccount.h"

struct SmtpData; //forward declaration

/*
Stores result of smtp transaction
*/
struct SmtpResult {
    bool success = false;
    std::string error;
};

class SmtpTransport {
public:
    virtual ~SmtpTransport() = default;

    virtual SmtpResult send(
        const SmtpData& message, 
        EmailAccount& account) = 0;
};
