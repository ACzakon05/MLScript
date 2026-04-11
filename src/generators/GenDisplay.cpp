#include "PythonGenerator.h"

std::any PythonGenerator::visitShowDataset(MLScriptParser::ShowDatasetContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    pythonCode << "print(" << varName << ")\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    pythonCode << "print(f'Columns in " << varName << ": {" << varName << ".columns.tolist()}')\n"; 

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowCount(MLScriptParser::ShowCountContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->FEATURES()) {
        pythonCode << "print(f'Numer of features in " << varName << ": {len(" << varName << ".columns)}')\n";
    } 
    else if (ctx->ROWS()) {
        pythonCode << "print(f'Numer of rows in " << varName << ": {len(" << varName << ")}')\n";
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowSingleRow(MLScriptParser::ShowSingleRowContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string rowNumber = ctx->INTEGER()->getText();

    pythonCode << "print('" << varName << " row at position " << rowNumber << ":')\n";
    pythonCode << "print(f'{" << varName << ".iloc[" << rowNumber << "]}')\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowMultipleRows(MLScriptParser::ShowMultipleRowsContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string lowerBound = ctx->INTEGER(0)->getText();
    std::string upperBound = std::to_string(std::stoi(ctx->INTEGER(1)->getText()) + 1);

    pythonCode << "print('" << varName << " rows from position " << lowerBound << " to position " << upperBound << ":')\n";
    pythonCode << "print(f'{" << varName << ".iloc[" << lowerBound << ":" << upperBound << "]}')\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowSingleFeature(MLScriptParser::ShowSingleFeatureContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->STRING()) {
        std::string featureName = ctx->STRING()->getText();
        pythonCode << "print(" << varName << "[" << featureName << "])\n";
    } 
    else if (ctx->INTEGER()) 
    {
        int featureIndex = std::stoi(ctx->INTEGER()->getText()) ;
        pythonCode << "print(" << varName << ".iloc[: , " << featureIndex - 1 << "])\n";
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowMultipleFeatures(MLScriptParser::ShowMultipleFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->columnList()) {
        pythonCode << "print(" << varName << "[[" << getColumnList(ctx->columnList()) << "]])\n";
    } 
    else if (ctx->INTEGER(0) && ctx->INTEGER(1))
    {
        std::string lowerBound = ctx->INTEGER(0)->getText();
        std::string upperBound = ctx->INTEGER(1)->getText();

        pythonCode << "print(" << varName << ".iloc[:, " << lowerBound << ":" << upperBound << "])\n"; 
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowAggFunc(MLScriptParser::ShowAggFuncContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string aggFunc = ctx->aggFunc()->getText();
    std::transform(aggFunc.begin(), aggFunc.end(), aggFunc.begin(), ::tolower);

    pythonCode << "print(" << varName << "[[" << getColumnList(ctx->columnList()) << "]]";
    pythonCode << ".agg(['" << aggFuncMap.at(aggFunc) << "']))\n";

    return visitChildren(ctx);
}
