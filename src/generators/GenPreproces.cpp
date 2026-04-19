#include "PythonGenerator.h"

std::any PythonGenerator::visitDropNaStat(MLScriptParser::DropNaStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Drop NA from " << dataSet << "\n";
    if (ctx->columnList()){
        pythonCode << dataSet << " = " << dataSet << ".dropna(subset=[" << getColumnList(ctx->columnList()) << "])\n";
    }
    else{
        pythonCode << dataSet << " = " << dataSet << ".dropna()\n";
    }
    return visitChildren(ctx);
}

std::any PythonGenerator::visitDropColumnStat(MLScriptParser::DropColumnStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Drop columns from " << dataSet << "\n";
    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        if (ctx->safeMode()) {
            // Safe mode: exclude target column if set
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "' if present\n";
                pythonCode << "columns_to_drop = [" << columns << "]\n";
                pythonCode << "if '" << target << "' in columns_to_drop: columns_to_drop.remove('" << target << "')\n";
                pythonCode << dataSet << " = " << dataSet << ".drop(columns=columns_to_drop)\n";
            } else {
                pythonCode << dataSet << " = " << dataSet << ".drop(columns=[" << columns << "])\n";
            }
        } else {
            pythonCode << dataSet << " = " << dataSet << ".drop(columns=[" << columns << "])\n";
        }
    }
    else if (ctx->whereClause()){
        std::string columnFilter = applyColumnConditions(ctx->whereClause(), dataSet);
        if (ctx->safeMode()) {
            // Safe mode: add condition to exclude target
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "'\n";
                // Modify columnFilter to add and col != target
                size_t ifPos = columnFilter.find("if ");
                if (ifPos != std::string::npos) {
                    columnFilter.insert(ifPos + 3, "col != '" + target + "' and ");
                }
            }
        }
        pythonCode << dataSet << " = " << dataSet << ".drop(columns=" << columnFilter << ")\n";
    }
    return visitChildren(ctx);
}

std::any PythonGenerator::visitNormalizeStat(MLScriptParser::NormalizeStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Normalize dataset (min-max scaling) "  << "\n";
    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        pythonCode<< "cols= [" << columns << "]\n";
        pythonCode<<dataSet << "[cols] = (" << dataSet << "[cols] - " << dataSet << "[cols].min()) / (" << dataSet << "[cols].max() - " << dataSet << "[cols].min())\n";

    }
    else{
        pythonCode << dataSet << " = (" << dataSet << " - " << dataSet << ".min()) / (" << dataSet << ".max() - " << dataSet << ".min())\n";    
    }

    return visitChildren(ctx);    
}

std::any PythonGenerator::visitStandardizeStat(MLScriptParser::StandardizeStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Standardize dataset (z-score normalization) "  << "\n";
    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        pythonCode<< "cols= [" << columns << "]\n";
        pythonCode<<dataSet << "[cols] = (" << dataSet << "[cols] - " << dataSet << "[cols].mean()) / " << dataSet << "[cols].std()\n";

    }
    else{
        pythonCode << dataSet << " = (" << dataSet << " - " << dataSet << ".mean()) / " << dataSet << ".std()\n";    
    }

    return visitChildren(ctx);    
}



