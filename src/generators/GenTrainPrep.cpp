#include "PythonGenerator.h"

std::any PythonGenerator::visitSetTargetStat(MLScriptParser::SetTargetStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER()->getText();
    std::string targetColWithQuotes = ctx->COL_NAME()->getText();
    std::string targetCol = targetColWithQuotes.substr(1, targetColWithQuotes.size() - 2); // Remove quotes
    targetColumns[dataSet] = targetCol;

    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();

    if (!symbolTable.exists(dataSet)) {
        diagnostics.reportSemanticError(line, col, "Trying to set target for " + dataSet + " which hasn't been declared.");
        return {};
    }

    mls::VariableType dataSetType = symbolTable.get(dataSet).type;
    if (dataSetType != mls::VariableType::DATASET) {
        diagnostics.reportSemanticWarning(line, col, "Trying to set target for " + dataSet + " of type " + mls::to_string(dataSetType) + " (expected DATASET).");
    }

    VariableMetadata meta = symbolTable.get(dataSet);
    meta.targetColumnName = targetCol;
    symbolTable.updateVariable(dataSet, meta);

    pythonCode << "# Marked '" << targetCol << "' as target for dataset '" << dataSet << "'\n";
    
    return visitChildren(ctx);
} 

//SPLIT my_dataset RATIO 80:20 INTO train_subset, test_subset WITH SEED 42, SHUFFLE true;
std::any PythonGenerator::visitSplitStat(MLScriptParser:: SplitStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER(0)->getText();
    std::string trainSubset = ctx->IDENTIFIER(1)->getText();
    std::string testSubset = ctx->IDENTIFIER(2)->getText();

    std::string ratioText = ctx->RATIO()->getText();
    size_t colonPos = ratioText.find(':');
    std::string trainRatio = ratioText.substr(0, colonPos);
    std::string testRatio = ratioText.substr(colonPos + 1);
    double testSize = std::stod(testRatio) / (std::stod(trainRatio) + std::stod(testRatio));

    // Opcjonalne seed ii shufle

    int seed =0;
    std::string shuffle = "True";
    if (ctx->SEED() && ctx->INTEGER()) {
        seed = std::stoi(ctx->INTEGER()->getText());
    }

    // obsługa TRUE / FALSE
    if (ctx->TRUE()) {
        shuffle = "True";
    } else if (ctx->FALSE()) {
        shuffle = "False";
    }

    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();

    if (!symbolTable.exists(dataSet)) {
        diagnostics.reportSemanticError(line, col, "Trying to split " + dataSet + " which does not exist."); 
        return {};
    }

    VariableMetadata sourceMeta = symbolTable.get(dataSet);

    if (sourceMeta.targetColumnName.empty()) {
        diagnostics.reportSemanticError(line, col, "You first must set target for " + dataSet + ".");
    }

    VariableMetadata childMeta = { sourceMeta.type, dataSet, sourceMeta.targetColumnName };
    symbolTable.addVariable(trainSubset, childMeta, line, col);
    symbolTable.addVariable(testSubset, childMeta, line, col);

    pythonCode << "# Split whole dataset into train and test\n";
    pythonHeader << "from sklearn.model_selection import train_test_split\n";
    
    pythonCode << trainSubset << ", " << testSubset << " = train_test_split(\n";
    pythonCode << "    " << dataSet << ",\n";
    pythonCode << "    test_size=" << testSize << ",\n";
    pythonCode << "    random_state=" << seed << ",\n";
    pythonCode << "    shuffle=" << shuffle << "\n";
    pythonCode << ")\n";

    return visitChildren(ctx);
}