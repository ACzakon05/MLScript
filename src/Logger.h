#pragma once
#include <string>

namespace Logger {
    constexpr const char* COLOR_YELLOW = "\033[33m";
    constexpr const char* COLOR_RED    = "\033[31m";
    constexpr const char* COLOR_RESET  = "\033[0m";

    const std::string WARNING_PREFIX = std::string(COLOR_YELLOW) + "[MLScript Compiler Warning] ";
    const std::string ERROR_PREFIX   = std::string(COLOR_RED) + "[MLScript Compiler Error] ";
}