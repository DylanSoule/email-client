#pragma once

#include <string>

/*
Defines any email address stored with email client
    This could include own email addresses or others
    Ex.
        name = John Smith
        address = john.smith@example.com
*/
struct emailAddress {
    std::string name;
    std::string address;
};