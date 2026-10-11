#pragma once

#include <string>

/*
Defines email account for client
    This will be a account the user owns or has access to
    Eventually will be more secure(ex not plaintext password storage)
*/
struct emailAccount {
    std::string id;

    std::string emailAddress;
    std::string name;

    std::string username;
    std::string password;

    enum class loginMethod { oauth2, simple };

    std::string smtpServer;
    u_int16_t smtpPort;

    std::string imapServer;
    u_int16_t imapPort;

};