#include "PythonGenerator.h"

// == Root ==

std::any PythonGenerator::visitProg(MLScriptParser::ProgContext *ctx) {
    pythonCode << "import pandas as pd\n";
    pythonCode << "from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load\n";

    return visitChildren(ctx);
}

// == Utils ==

std::string PythonGenerator::getColumnList(MLScriptParser::ColumnListContext *ctx) {
    std::string list;
    auto strings = ctx->COL_NAME();

    for (size_t i = 0; i < strings.size(); ++i) {
        list += strings[i]->getText();
        if (i < strings.size() - 1) list += ", ";
    }
    
    return list;
}

std::string PythonGenerator::applyWhereConditions(MLScriptParser::WhereClauseContext *ctx) {
    if (!ctx) {
        return "";
    }
    
    std::any pandasWhereConditions = visit(ctx->condition());

    if (pandasWhereConditions.has_value()) {
        std::string pandasWhereConditionsString = std::any_cast<std::string>(pandasWhereConditions);

        if (!pandasWhereConditionsString.empty()) {
            return std::string(".where(" + pandasWhereConditionsString + ")");
        }
    }

    return "";
}