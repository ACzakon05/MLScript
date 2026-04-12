#include "PythonGenerator.h"

std::any PythonGenerator::visitShowStat(MLScriptParser::ShowStatContext *ctx) {
    pythonCode << "print(";

    visitChildren(ctx);

    pythonCode << ")\n";

    return {};
}

std::any PythonGenerator::visitShowDataset(MLScriptParser::ShowDatasetContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    currentVarName = varName;

    pythonCode << varName;
    pythonCode << applyWhereConditions(ctx->whereClause());

    return {};
}

std::any PythonGenerator::visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    currentVarName = varName;

    pythonCode << "f'Columns in " << varName << ": {" << varName << ".columns.tolist()}'"; 

    return {};
}

std::any PythonGenerator::visitShowCount(MLScriptParser::ShowCountContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    currentVarName = varName;

    if (ctx->FEATURES()) {
        pythonCode << "f'Numer of features in " << varName << ": {len(" << varName << ".columns)}'";
    } 
    else if (ctx->ROWS()) {
        pythonCode << "f'Numer of rows in " << varName << ": {len(" << varName << ")}'";
    }

    return {};
}

std::any PythonGenerator::visitShowSingleRow(MLScriptParser::ShowSingleRowContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string rowNumber = ctx->INTEGER()->getText();

    currentVarName = varName;

    pythonCode << varName << ".iloc[" << rowNumber << "]";

    return {};
}

std::any PythonGenerator::visitShowMultipleRows(MLScriptParser::ShowMultipleRowsContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string lowerBound = ctx->INTEGER(0)->getText();
    std::string upperBound = std::to_string(std::stoi(ctx->INTEGER(1)->getText()) + 1);
    
    currentVarName = varName;

    pythonCode << varName;
    pythonCode << applyWhereConditions(ctx->whereClause());
    pythonCode << ".iloc[" << lowerBound << ":" << upperBound << "]";

    return {};
}

std::any PythonGenerator::visitShowSingleFeature(MLScriptParser::ShowSingleFeatureContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    currentVarName = varName;

    if (ctx->COL_NAME()) {
        std::string featureName = ctx->COL_NAME()->getText();
        pythonCode << varName << "[" << featureName << "]";
    } 
    else if (ctx->INTEGER()) 
    {
        int featureIndex = std::stoi(ctx->INTEGER()->getText()) ;
        pythonCode << varName << ".iloc[: , " << featureIndex - 1 << "]";
    }

    return {};
}

std::any PythonGenerator::visitShowMultipleFeatures(MLScriptParser::ShowMultipleFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    currentVarName = varName;

    if (ctx->columnList()) {
        pythonCode << varName << "[[" << getColumnList(ctx->columnList()) << "]]";
    } 
    else if (ctx->INTEGER(0) && ctx->INTEGER(1))
    {
        std::string lowerBound = ctx->INTEGER(0)->getText();
        std::string upperBound = ctx->INTEGER(1)->getText();

        pythonCode << varName << ".iloc[:, " << lowerBound << ":" << upperBound << "]"; 
    }

    return {};
}

std::any PythonGenerator::visitShowAggFunc(MLScriptParser::ShowAggFuncContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string aggFunc = ctx->aggFunc()->getText();

    currentVarName = varName;

    std::transform(aggFunc.begin(), aggFunc.end(), aggFunc.begin(), ::tolower);

    pythonCode << varName << "[[" << getColumnList(ctx->columnList()) << "]]";
    pythonCode << applyWhereConditions(ctx->whereClause());
    pythonCode << ".agg(['" << aggFuncMap.at(aggFunc) << "'])";

    return {};
}
