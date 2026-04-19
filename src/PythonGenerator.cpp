#include "PythonGenerator.h"

// == Root ==

std::any PythonGenerator::visitProg(MLScriptParser::ProgContext *ctx) {
    pythonHeader << "import pandas as pd\n";
    pythonHeader << "from mlscript_runtime.error_handling.safe_execute_load import safe_execute_load\n";

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

std::string PythonGenerator::getDatasetExistenceCheck(const std::string& dataSet) {
    return "if '" + dataSet + "' not in globals() and '" + dataSet + "' not in locals():\n"
           "    raise NameError(\"Dataset " + dataSet + " not found\")\n";
}

std::string PythonGenerator::getColumnsExistenceCheck(const std::string& dataSet, const std::string& columnsVar) {
    return columnsVar + " = list(" + columnsVar + ")\n"
           "missing = [c for c in " + columnsVar + " if c not in " + dataSet + ".columns]\n"
           "if missing:\n"
           "    raise KeyError(f\"Columns not found: {missing}\")\n";
}

std::string PythonGenerator::applyColumnConditions(MLScriptParser::WhereClauseContext *ctx, const std::string& dataSet) {
    if (!ctx) {
        return "[]";
    }
    
    isColumnContext = true;
    std::any condition = visit(ctx->condition());
    isColumnContext = false;

    if (!condition.has_value()) {
        return "[]";
    }

    std::string condStr = std::any_cast<std::string>(condition);

    if (condStr.empty()) {
        return "[]";
    }

    // Replace DATASET placeholder with actual dataset name
    size_t pos = 0;
    while ((pos = condStr.find("DATASET", pos)) != std::string::npos) {
        condStr.replace(pos, 7, dataSet);
        pos += dataSet.length();
    }

    return "[col for col in " + dataSet + ".columns if " + condStr + "]";
}