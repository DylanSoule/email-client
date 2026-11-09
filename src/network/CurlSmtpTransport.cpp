#include <curl/curl.h>
#include <algorithm>
#include <string>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <iostream> // for testing purposes

#include "SmtpTransport.h"
#include "CurlSmtpTransport.h"
#include "CurlMulti.h"
#include "../email/EmailAccount.h"


CurlSmtpTransport::CurlSmtpTransport(CurlMulti& multi) : multi_(multi) {}

SmtpResult CurlSmtpTransport::prepare (
    const SmtpData& message, 
    EmailAccount& account) {
    (void)account;

    SmtpResult res; 

    auto transaction = std::make_unique<CurlMulti::Transaction>();
    transaction->easyHandle = curl_easy_init();
    if(!transaction->easyHandle) {
        throw std::runtime_error("Failed to init easy handle");
    }

    transaction->payload.message =
        "Date: " + message.date + "\r\n" +
        "To: " + message.recipients + "\r\n" +
        "From: " + message.sender + "\r\n" +
        (message.carbonCopy.empty() ? 
        "" : "Cc: " + message.carbonCopy + "\r\n") +
        "Message-ID: " + "<" + message.messageId + ">" + "\r\n" + 
        "Subject: " + message.subject + "\r\n" +
        "\r\n" +
        message.contents + "\r\n";

    std::cout << transaction->payload.message;
    
    for (auto &addr : message.recipientsList) {
        transaction->recipients = curl_slist_append(transaction->recipients, addr.address.c_str());
    }
    for (auto &addr : message.carbonCopyList) {
        transaction->recipients = curl_slist_append(transaction->recipients, addr.address.c_str());
    }
    for (auto &addr : message.blindCarbonCopyList) {
        transaction->recipients = curl_slist_append(transaction->recipients, addr.address.c_str());
    }


    // Set up credentials - Mail server - username - Password
    curl_easy_setopt(transaction->easyHandle, CURLOPT_URL, message.serverUrl.c_str());
    curl_easy_setopt(transaction->easyHandle, CURLOPT_USERNAME, message.username.c_str());
    curl_easy_setopt(transaction->easyHandle, CURLOPT_PASSWORD, message.password.c_str());

    // Ensure TLS usage
    curl_easy_setopt(transaction->easyHandle, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    /*
    Set up libcurl for sending email by passing need parameters
        - email
        - receiving email
        - function that determines the bytes left in the message
        - info struct
        - configure data transfer
    */

    std::cout << "\n\n\n\n\n" + account.emailAddress + "\n\n\n\n\n";
    curl_easy_setopt(transaction->easyHandle, CURLOPT_MAIL_FROM, account.emailAddress.c_str());
    curl_easy_setopt(transaction->easyHandle, CURLOPT_MAIL_RCPT, transaction->recipients);
    curl_easy_setopt(transaction->easyHandle, CURLOPT_READFUNCTION, CurlSmtpTransport::payloadCallBack);
    curl_easy_setopt(transaction->easyHandle, CURLOPT_READDATA, &transaction->payload);
    curl_easy_setopt(transaction->easyHandle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(transaction->easyHandle, CURLOPT_VERBOSE, 1L);

    const std::uint64_t transferId = multi_.addTransaction(std::move(transaction));

    const CURLcode performCode = multi_.perform();
    if (performCode != CURLE_OK) {
        res.success = false;
        res.error = curl_easy_strerror(performCode);
        return res;
    }

    auto transferResult = multi_.takeResult(transferId);
    if (!transferResult.has_value()) {
        res.success = false;
        res.error = "Transfer finished without an available result";
        return res;
    }

    res.success = transferResult->success;
    res.error = transferResult->error;
    return res;
}

size_t CurlSmtpTransport::payloadCallBack(char *buffer, size_t size, size_t nmemb, void *userp) {
    auto* uploadStatus = static_cast<CurlMulti::Payload*>(userp);
    size_t room = size * nmemb;
    if (room < 1) return 0;

    size_t remainingBytes = uploadStatus->message.size() - uploadStatus->bytesRead;
    size_t bytesToCopy = std::min(room, remainingBytes);

    if (bytesToCopy > 0) {
        std::memcpy(buffer, uploadStatus->message.c_str() + uploadStatus->bytesRead, bytesToCopy);
        uploadStatus->bytesRead += bytesToCopy;
    }

    return bytesToCopy;
}