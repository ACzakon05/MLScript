#include "PythonGenerator.h"

// == Root ==

std::any PythonGenerator::visitProg(MLScriptParser::ProgContext *ctx) {
    pythonCode << "import pandas as pd\n";

    return visitChildren(ctx);
}

// == Utils ==

std::string PythonGenerator::getColumnList(MLScriptParser::ColumnListContext *ctx) {
    std::string list;
    auto strings = ctx->STRING();

    for (size_t i = 0; i < strings.size(); ++i) {
        list += strings[i]->getText();
        if (i < strings.size() - 1) list += ", ";
    }
    
    return list;
}