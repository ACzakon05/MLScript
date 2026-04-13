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
    std:: string column = ctx->COL_NAME()->getText();
    pythonCode << "# Drop column(s) from " << dataSet << "\n";
    pythonCode << dataSet << " = " << dataSet << ".drop(columns=[" << column << "])\n";
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



