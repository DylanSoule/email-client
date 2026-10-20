#pragma once;

#include <string>

#include "SmtpTransport.h"
#include "../transfer/SmtpClient.h"

/*
Derived from SmtpTransport class
Build for interfacing with libcurl for smtp
*/
class CurlSmtpTransport : public SmtpTransport {
    public:
        SmtpResult send(const const SmtpData& message) override;
};