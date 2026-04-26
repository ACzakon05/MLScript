#include "PythonGenerator.h"
#include "CustomErrorListener.h"

std::any PythonGenerator::visitTrainStat(MLScriptParser::TrainStatContext *ctx) {
    std::string modelName = ctx->modelName->getText();
    std::string trainSet = ctx->trainSet->getText();

    if (!symbolTable.exists(modelName)) {
        size_t line = ctx->getStart()->getLine();
        size_t col = ctx->getStart()->getCharPositionInLine();

        diagnostics.reportSemanticError(line, col, "Model " + modelName + " has not been created.");
        return {};
    }

    mls::VariableType modelType = symbolTable.get(modelName).type;
    if (modelType != mls::VariableType::MODEL) {
        size_t line = ctx->getStart()->getLine();
        size_t col = ctx->getStart()->getCharPositionInLine();

        diagnostics.reportSemanticWarning(line, col, "Trying to train object " + modelName + " of type " + mls::to_string(modelType) + " (expected MODEL).");
    }

    std::string targetCol = symbolTable.get(trainSet).targetColumnName;
    
    pythonCode << "X_train = " << trainSet << ".drop(columns=['" << targetCol << "'])\n";
    pythonCode << "y_train = " << trainSet << "['" << targetCol << "']\n";
    pythonCode << modelName << ".fit(X_train, y_train)\n";

    return {};
}