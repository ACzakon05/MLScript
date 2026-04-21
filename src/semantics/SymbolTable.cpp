#include "SymbolTable.h"

void SymbolTable::addVariable(const std::string& name, VariableMetadata meta, size_t line, size_t col) {
    auto it = symbols.find(name);

    if (it != symbols.end()) {
        mls::VariableType oldType = symbols.at(name).type;
        std::string message = "Variable reasignment for variable " + name + " [" + mls::to_string(oldType) + " -> " + mls::to_string(meta.type) + "]";
        diagnostics.reportSemanticWarning(line, col, message);

        it->second = std::move(meta);
    } else {
        symbols.emplace(name, std::move(meta));
    }
}

bool SymbolTable::exists(const std::string& name) {
    auto it = symbols.find(name);

    return it != symbols.end();
}

VariableMetadata SymbolTable::get(const std::string& name) {
    if (!exists(name)) {
        return VariableMetadata{};
    }

    return symbols.at(name);
}