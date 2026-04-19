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
    pythonCode << getDatasetExistenceCheck(dataSet);

    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        pythonCode << "columns_to_drop = [" << columns << "]\n";
        pythonCode << getColumnsExistenceCheck(dataSet, "columns_to_drop");

        if (ctx->safeMode()) {
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "' if present\n";
                pythonCode << "if '" << target << "' in columns_to_drop: columns_to_drop.remove('" << target << "')\n";
            }
        }

        pythonCode << dataSet << " = " << dataSet << ".drop(columns=columns_to_drop)\n";
    }
    else if (ctx->whereClause()){
        std::string columnFilter = applyColumnConditions(ctx->whereClause(), dataSet);
        pythonCode << "columns_to_drop = " << columnFilter << "\n";
        pythonCode << getColumnsExistenceCheck(dataSet, "columns_to_drop");

        if (ctx->safeMode()) {
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "'\n";
                pythonCode << "columns_to_drop = [col for col in columns_to_drop if col != '" << target << "']\n";
            }
        }

        pythonCode << dataSet << " = " << dataSet << ".drop(columns=columns_to_drop)\n";
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



