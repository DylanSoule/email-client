#pragma once

#include <string>

#include "../email/EmailAccount.h"

struct SmtpData; //forward declaration

class SmtpTransport {
public:
    virtual ~SmtpTransport() = default;

    virtual void prepare(
        const SmtpData& message, 
        EmailAccount& account) = 0;
};
