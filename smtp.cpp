#include <curl/curl.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

class EmailClient {
public:
    /*
    Constructor function
    - casts constructor params to class attributes
    - inits libcurl and makes sure that it doesn't throw an error
    */
    EmailClient(
        std::string smtpServer,
        std::string imapServer,
        std::string senderEmail, 
        std::string senderName,
        std::string senderPswd,
        std::string senderUname  = "")
    : smtpServer_(std::move(smtpServer)), 
    imapServer_(std::move(imapServer)),
    senderEmail_(std::move(senderEmail)), 
    senderName_(std::move(senderName)),
    senderUname_(senderUname.empty() ? senderEmail_ : std::move(senderUname)),
    senderPswd_(std::move(senderPswd)) {}

    bool sendMail(
        const std::string& receiverEmail, 
        const std::string& subject, 
        const std::string& body,
        const std::string& receiverName = "" ) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            return false;
        }

        std::string fromHeader = senderName_.empty() ? 
            "<" + senderEmail_ + ">"
            : senderName_ + " <" + senderEmail_ + ">";
        
        std::string toHeader = receiverName.empty() ? 
            "<" + receiverEmail + ">"
            : receiverName + " <" + receiverEmail + ">";
            

        std::string rawMessage = 
            "To: " + toHeader + "\r\n" +
            "From: " + fromHeader + "\r\n" +
            // "CC: " + ccHeader + "\r\n" +
            "Subject: " + subject + "\r\n" +
            "\r\n" + // Empty line divides headers from body
            body + "\r\n";

        Payload messageState{ &rawMessage, 0 };

        struct curl_slist* recipients = nullptr;
        recipients = curl_slist_append(recipients, receiverEmail.c_str());

        // Set up credentials - Mail server - username - Password
        curl_easy_setopt(curl, CURLOPT_URL, smtpServer_.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, senderUname_.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, senderPswd_.c_str());
        
        // Ensure TLS usage
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        /*
        Set up libcurl for sending email by passing need parameters
            - email
            - receiving email
            - function that determines the bytes left in the message
            - info struct
            - configure data transfer
        */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, senderEmail_.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, EmailClient::payloadCallBack);
        curl_easy_setopt(curl, CURLOPT_READDATA, &messageState);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        //send the message
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        //cleanup
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        return res == CURLE_OK;
    }

    bool fetchMail() {
        CURL *curl = curl_easy_init();
        if (!curl) {
            return false;
        }

        // Set up credentials
        curl_easy_setopt(curl, CURLOPT_USERNAME, senderUname_.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, senderPswd_.c_str());
        std::string full = "imaps://" + std::string(imapServer_) + "/INBOX/;MAILINDEX=63";
        curl_easy_setopt(curl, CURLOPT_URL, full.c_str());

        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


        // Force SSL/TLS
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        //perform
        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        //cleanup
        curl_easy_cleanup(curl);

        return res == CURLE_OK;
    }

private:
    // Declare private variables to store constructor parameters
    std::string senderEmail_;
    std::string senderName_;
    std::string senderUname_;
    std::string senderPswd_;
    std::string smtpServer_;
    std::string imapServer_;

    struct Payload {
        const std::string* message;
        size_t bytesRead;
    };

    static size_t payloadCallBack(char *buffer, size_t size, size_t nmemb, void *userp) {
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
};



int main() {
    // inits global libcurl interaction, done in main so class destruction doesn't close libcurl
    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    if (result != CURLE_OK) {
        std::cerr << "curl_global_init failed: " << curl_easy_strerror(result) << "\n";
        return 1;
    }

    // multi line body
    std::string body = R"(Hello Email Receiver,

    This email was sent via a c++ script using libcurl!)";

    // Init class with sender data, and send email using receiver data
    EmailClient icloud("smtp.mail.me.com:587", "imap.mail.me.com:993", "dylan.soule@icloud.com", "Dylan Soule", "mzma-slqv-ncog-jrsu");
    // icloud.sendMail("2141247@jeffcoschools.us", "First email through c++", body);
    icloud.fetchMail();

    // clean up libcurl globally
    curl_global_cleanup();
    return 0;
}