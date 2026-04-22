#pragma once
#include <string>
#include <unordered_map>

namespace mlscript {

    /**
     * @brief Enum with all file extensions available in MLScript.
     */
    enum fileExtension {
        CSV,
        SQL,
        JSON,
        PKL
    };

    /**
     * @brief Stores load options for LOAD command.
     */
    struct LoadConfig {
        std::string filePath;
        fileExtension fileFormat = fileExtension::CSV;
        std::string delimiter;
        std::string headerOption;
        std::string columnsToKeep;
        std::string columnsToDiscard;
        std::string nrows;
        std::string orient;
    };

    /**
     * @brief Maps lowercase file formats from grammar to pandas equivalent.
     */
    inline const std::unordered_map<std::string, std::string> fileFormatMap = {
        {"csv", "csv"},
        {"sql", "sql_table"},
        {"pkl", "pickle"},
        {"json", "json"},
        {"html", "html"}
    };

    /**
     * @brief Maps lowercase aggregation function keywords from grammar to pandas functions.
     */
    inline const std::unordered_map<std::string, std::string> aggFuncMap = {
        {"mean", "mean"},
        {"max", "max"},
        {"min", "min"},
        {"sum", "sum"},
        {"median", "median"},
        {"prod", "prod"},
        {"std", "std"},
        {"var", "var"},
        {"unique_vals", "unique"},
        {"unique_count", "nunique"}
    };

}