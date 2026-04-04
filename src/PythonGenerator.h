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

        if (ctx->KEEP()) {
            pythonCode << varName << " = " << varName << "[[" << getColumnList(ctx->columnList()) << "]]\n";
        } else if (ctx->WITHOUT()) {
            pythonCode << varName << " = " << varName << ".drop(columns=[" << getColumnList(ctx->columnList()) << "])\n";
        }

        return visitChildren(ctx);
    }

    std::any visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();

        pythonCode << "print(f'Columns in " << varName << ": {" << varName << ".columns.tolist()}')\n"; 

        return visitChildren(ctx);
    }

    std::any visitShowCount(MLScriptParser::ShowCountContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();

        if (ctx->FEATURES()) {
            pythonCode << "print(f'Numer of features in " << varName << ": {len(" << varName << ".columns)}')\n";
        } 
        else if (ctx->ROWS()) {
            pythonCode << "print(f'Numer of rows in " << varName << ": {len(" << varName << ")}')\n";
        }

        return visitChildren(ctx);
    }

private:
    std::string getColumnList(MLScriptParser::ColumnListContext *ctx) {
        std::string list;
        auto strings = ctx->STRING();

        for (size_t i = 0; i < strings.size(); ++i) {
            list += strings[i]->getText();
            if (i < strings.size() - 1) list += ", ";
        }
        
        return list;
    }
};