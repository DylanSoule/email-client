#pragma once;

#include <string>

#include "../transfer/SmtpClient.h"

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

        virtual SmtpResult send(const SmtpData& message) = 0;
};
