#pragma once
#include "Logger.h"
#include "antlr4-runtime.h"
#include <iostream>

class CustomErrorListener :  public antlr4::BaseErrorListener {
public:
    bool hasSyntaxError = false;
    int semanticWarningCount = 0;
    int semanticErrorCount = 0;

    void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                     size_t line,
                     size_t charPositionInLine,
                     const std::string &msg,
                     std::exception_ptr e) override 
                     {
        hasSyntaxError = true;
        
        std::cerr << Logger::ERROR_PREFIX << "at line " << line << ", column " << charPositionInLine << ":\n" << Logger::COLOR_RESET;
        std::cerr << " -> " << msg << "\n\n";
    }

    void reportSemanticWarning(size_t line, size_t col, const std::string& msg) {
        semanticWarningCount++;
        std::cerr << Logger::WARNING_PREFIX << "at line " << line << ", column " << col << ":\n" << Logger::COLOR_RESET;
        std::cerr << " -> " << msg << "\n\n";
    }

    void reportSemanticError(size_t line, size_t col, const std::string& msg) {
        semanticErrorCount++;
        std::cerr << Logger::ERROR_PREFIX << "at line " << line << ", column " << col << ":\n" << Logger::COLOR_RESET;
        std::cerr << " -> " << msg << "\n\n";
    }
};