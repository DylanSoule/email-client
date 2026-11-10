#pragma once

class CurlGlobal {
    /*
    RAII class that takes care of setting up and destruction curl global on program init and destruction
    */
    public:
        CurlGlobal();

        ~CurlGlobal();

        CurlGlobal(const CurlGlobal&) = delete;
        CurlGlobal& operator=(const CurlGlobal&) = delete;
};