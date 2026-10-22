#include <curl/curl.h>
#include <string>
#include <cstring>

#include "SmtpTransport.h"
#include "CurlSmtpTransport.h"

SmtpResult CurlSmtpTransport::send (const SmtpData& message) {
    SmtpResult res; 

    // Initialize easy handle and check if it exists
    CURL *easy = curl_easy_init();
    if(!easy) {
        res.error = "Failed to init";
        return res;
    }

    std::string rawMessage =
        "Date: " + message.date + "\r\n" +
        "To: ";
}

size_t CurlSmtpTransport::payloadCallBack(char *buffer, size_t size, size_t nmemb, void *userp) {
    auto* uploadStatus = static_cast<CurlSmtpTransport::Payload*>(userp);
    size_t room = size * nmemb;
    if (room < 1) return 0;

    size_t remainingBytes = uploadStatus->message->size() - uploadStatus->bytesRead;
    size_t bytesToCopy = std::min(room, remainingBytes);

    if (bytesToCopy > 0) {
        std::memcpy(buffer, uploadStatus->message->c_str() + uploadStatus->bytesRead, bytesToCopy);
        uploadStatus->bytesRead += bytesToCopy;
    }

    return bytesToCopy;
}