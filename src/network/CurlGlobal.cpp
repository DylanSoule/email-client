#include <curl/curl.h>
#include <string>
#include <stdexcept>

#include <CurlGlobal.h>

CurlGlobal::CurlGlobal(){
    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    if (result != CURLE_OK) {
        throw std::runtime_error("Failed to init libcurl");
    }
}

CurlGlobal::~CurlGlobal(){
    curl_global_cleanup();
}