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
)

target_include_directories(email-client
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(email-client
    PRIVATE
        PkgConfig::CURL
        PkgConfig::VMIME
)
