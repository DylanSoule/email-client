#include <curl/curl.h>
#include <string>
#include <cstring>

#include "SmtpTransport.h"
#include "CurlSmtpTransport.h"
#include "CurlMulti.h"
#include "../email/EmailAccount.h"


CurlSmtpTransport::CurlSmtpTransport(CurlMulti multi) : multi_(multi) {}

SmtpResult CurlSmtpTransport::send (
    const SmtpData& message, 
    EmailAccount& account) {
    SmtpResult res; 

    // Initialize easy handle and check if it exists
    CURL *easy = curl_easy_init();
    if(!easy) {
        res.error = "Failed to init";
        return res;
    }

    std::string rawMessage =
        "Date: " + message.date + "\r\n" +
        "To: " + message.sender + "\r\n" +
        "From: " + message.sender + "\r\n" +
        (message.carbonCopy.empty() ? 
        "" : "Cc: " + message.carbonCopy + "\r\n") +
        "Message-ID: " + "<" + message.messageId + ">" + "\r\n" + 
        "Subject: " + message.subject + "\r\n" +
        "\r\n" +
        message.contents + "\r\n" +
        "Check RFC 5322.\r\n";
    
    Payload messageState{ &rawMessage, 0 };

    struct curl_slist *recipients = nullptr;
    
    for (auto &addr : message.recipientsList) {
        recipients = curl_slist_append(recipients, addr.address.c_str());
    }
    for (auto &addr : message.carbonCopyList) {
        recipients = curl_slist_append(recipients, addr.address.c_str());
    }
    for (auto &addr : message.blindCarbonCopyList) {
        recipients = curl_slist_append(recipients, addr.address.c_str());
    }


    // Set up credentials - Mail server - username - Password
    curl_easy_setopt(easy, CURLOPT_URL, message.serverUrl.c_str());
    curl_easy_setopt(easy, CURLOPT_USERNAME, message.username.c_str());
    curl_easy_setopt(easy, CURLOPT_PASSWORD, message.password.c_str());

    // Ensure TLS usage
    curl_easy_setopt(easy, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    /*
    Set up libcurl for sending email by passing need parameters
        - email
        - receiving email
        - function that determines the bytes left in the message
        - info struct
        - configure data transfer
    */
    curl_easy_setopt(easy, CURLOPT_MAIL_FROM, message.sender.c_str());
    curl_easy_setopt(easy, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, CurlSmtpTransport::payloadCallBack);
    curl_easy_setopt(easy, CURLOPT_READDATA, &messageState);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // Add handle to multi
    
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