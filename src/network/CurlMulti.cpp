#include <curl/curl.h>
#include <stdexcept>

#include "CurlMulti.h"

CurlMulti::CurlMulti() {
    multi_ = curl_multi_init();
    
    if (!multi_) {
        throw std::runtime_error("Failed to init libcurl multi");
    }
}

CurlMulti::~CurlMulti() {
    CURL** handles = curl_multi_get_handles(multi_);
    if (*handles != nullptr) {
        for (CURL** handle = handles; *handle !=nullptr; ++handle) {
            curl_multi_remove_handle(multi_, *handle);
            curl_easy_cleanup(*handle);
        }
    }

    curl_multi_cleanup(multi_);
}

void CurlMulti::addHandle(CURL* easy_handle) {
    
}