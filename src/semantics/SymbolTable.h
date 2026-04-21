#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

#include "CustomErrorListener.h"
#include "VariableType.h"

struct VariableMetadata {
    mls::VariableType type;
    std::string originDataset;
};

class SymbolTable {
public:
    SymbolTable(CustomErrorListener& diag) : diagnostics(diag) {}

    void addVariable(const std::string& name, VariableMetadata meta, size_t line, size_t col);
    bool exists(const std::string& name);
    VariableMetadata get(const std::string& name);

private:    
    std::unordered_map<std::string, VariableMetadata> symbols;
    CustomErrorListener& diagnostics;
};