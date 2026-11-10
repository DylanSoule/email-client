cmake_minimum_required(VERSION 3.20)

project(
    EmailClient
    DESCRIPTION "C++ email client"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for tools such as clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(VMIME REQUIRED IMPORTED_TARGET vmime)

add_executable(email-client
    src/main.cpp

    src/network/CurlGlobal.h
    src/network/CurlGlobal.cpp
    src/network/CurlMulti.h
    src/network/CurlMulti.cpp

    src/transfer/SmtpClient.cpp
    src/transfer/SmtpClient.h

    src/email/EmailAccount.h
    src/email/EmailAddress.h
    src/email/EmailMessage.h
    src/email/EmailMessage.cpp

    src/network/SmtpTransport.h
    src/network/CurlSmtpTransport.cpp
    src/network/CurlSmtpTransport.h
)

target_include_directories(email-client
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(email-client
    PRIVATE
        PkgConfig::CURL
        PkgConfig::VMIME
)
