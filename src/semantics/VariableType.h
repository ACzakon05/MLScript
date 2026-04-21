#pragma once

#include <string>
#include <ostream>
#include <sstream>

namespace mls {
    enum class VariableType {
        DATASET,
        MODEL,
        SERIES
    };

    inline std::ostream& operator <<(std::ostream& os, const VariableType& type) {
        switch (type)
        {
        case VariableType::DATASET:
            return os << "DATASET";
        case VariableType::MODEL:
            return os << "MODEL";
        case VariableType::SERIES:
            return os << "SERIES";
        default:
            return os << "UNKNOWN TYPE";
        }
    }

    inline std::string to_string(VariableType type) {
        std::ostringstream oss;
        oss << type;
        return oss.str();
    }
}