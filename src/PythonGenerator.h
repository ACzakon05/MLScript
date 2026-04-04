#pragma once
#include "MLScriptBaseVisitor.h"
#include <sstream>
#include <string>
#include <iostream>

class PythonGenerator : public MLScriptBaseVisitor {
public:
    std::stringstream pythonCode;

    std::any visitProg(MLScriptParser::ProgContext *ctx) override {
        pythonCode << "import pandas as pd\n";

        return visitChildren(ctx);
    }

    std::any visitLoadStat(MLScriptParser::LoadStatContext *ctx) override {
        std::string fileName = ctx->STRING()->getText();
        std::string varName = ctx->IDENTIFIER()->getText();

        pythonCode << varName << " = pd.read_csv(" << fileName << ")\n";

        return visitChildren(ctx);
    }
};