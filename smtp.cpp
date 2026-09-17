#include <curl/curl.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

class SMTPClient {
public:
    /*
    Constructor function
    - casts constructor params to class attributes
    - inits libcurl and makes sure that it doesn't throw an error
    */
    SMTPClient(std::string smtpServer, std::string senderAddress)
    : server_(std::move(smtpServer)), sender_(std::move(senderAddress)){
        CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
        if(result != CURLE_OK){
            throw std::runtime_error("curl_global_init failed: " + std::string(curl_easy_strerror(result)));
        }
    }
    // Deconstructor function cleans up libcurl
    ~SMTPClient() {
        curl_global_cleanup();
    }

    bool sendMail (){
        
    }


private:
    // Declare private variables to store constructor parameters
    std::string sender_;
    std::string server_;
};

int main() {
    ;
}