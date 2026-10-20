#pragma once

#include <curl/curl.h>

/*
RAII class to initialize and cleanup curl multi interface
- Also adds and removes easy handles for parallel performing
*/
class CurlMulti {
    public:
        CurlMulti();

        ~CurlMulti();

        CurlMulti(const CurlMulti&) = delete;
        CurlMulti& operator=(const CurlMulti&) = delete;

        void addHandle(CURL* easy_handle);

        void removeHandle(CURL* easy_handle);
    private:
        CURLM* multi_;
};