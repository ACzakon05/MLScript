#pragma once
#include <iostream>

namespace ModelParamParseUtil {
    template <typename T>
    std::pair<std::string, std::string> parseBooleanParam(std::string paramName, T *ctx) {
        if (ctx->val == nullptr) {
            return {paramName, "True"};
        }

        std::any paramValAny = ctx->val->getText();
        std::string paramValString = std::any_cast<std::string>(paramValAny);
        std::transform(paramValString.begin(), paramValString.end(), paramValString.begin(), [](unsigned char c){ return std::tolower(c); });
        
        if (paramValString == "true") {
            return {paramName, "True"};
        }

        return {paramName, "False"};
    }

    template <typename T>
    std::pair<std::string, std::string> parseNumberParam(std::string paramName, T *ctx) {
        std::any paramValAny = ctx->val->getText();
        std::string paramValString = std::any_cast<std::string>(paramValAny);

        return {paramName, paramValString};
    }
};