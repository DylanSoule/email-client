#pragma once

#include <string>

#include "SmtpTransport.h"
#include "../transfer/SmtpClient.h"
#include "../email/EmailAccount.h"
#include "CurlMulti.h"

/*
Derived from SmtpTransport class
Build for interfacing with libcurl for smtp
*/
class CurlSmtpTransport : public SmtpTransport {
    public:
        CurlSmtpTransport(CurlMulti multi);

        SmtpResult send (
            const SmtpData& message, 
            EmailAccount& account) override;

    private:
        CurlMulti multi_;

        struct Payload {
            const std::string* message;
            size_t bytesRead;
        };

        // Gives libcurl the bytes that it needs to copy for each packet
        static size_t payloadCallBack(char *buffer, size_t size, size_t nmemb, void *userp);
};