#include <curl/curl.h>
#include <cstring>

#include "CurlSmtpTransport.h"

SmtpResult CurlSmtpTransport::send (const SmtpData& message) {
    
}

size_t CurlSmtpTransport::payloadCallBack(char *buffer, size_t size, size_t nmemb, void *userp) {
    auto* messageState = static_cast<Payload*>(userp);
    size_t room = size * nmemb;
    if (room < 1) return 0;

    size_t remainingBytes = messageState->message->size() - messageState->bytesRead;
    size_t bytesToCopy = std::min(room, remainingBytes);

    if (bytesToCopy > 0) {
        std::memcpy(buffer, messageState->message->c_str() + messageState->bytesRead, bytesToCopy);
        messageState->bytesRead += bytesToCopy;
    }

    return bytesToCopy;
}