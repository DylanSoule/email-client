#pragma once

#include <string>

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

    virtual SmtpResult send(const SmtpData& message) = 0;
};
