#include "PythonGenerator.h"

std::any PythonGenerator::visitPredictStat(
    MLScriptParser::PredictStatContext *ctx
)
{
    std::string modelName=ctx->modelName->getText();
    std::string dataset=ctx->dataSet->getText();
    std::string output=ctx->predictions->getText();

    size_t line = ctx->getStart()->getLine();
    size_t col  = ctx->getStart()->getCharPositionInLine();

    //model check
    if (!symbolTable.exists(modelName)) {
        diagnostics.reportSemanticError(line, col,
            "Model " + modelName + " does not exist.");
        return {};
    }

    VariableMetadata modelMeta = symbolTable.get(modelName);

    if (modelMeta.type != mls::VariableType::MODEL) {
        diagnostics.reportSemanticError(line, col,
            modelName + " is not a MODEL.");
        return {};
    }

    if (!modelMeta.isTrained) {
        diagnostics.reportSemanticError(line, col,
            "Model " + modelName + " is not trained.");
        return {};
    }

    //dataSet check
    if(!symbolTable.exists(dataset)){
        diagnostics.reportSemanticError(line,col,"Dataset " + dataset + " does not exist.");
        return {};
    }

    
    // FEATURE SCHEMA CHECK
    if (modelMeta.trainedFeatureColumnsVariable.empty()) {
        diagnostics.reportSemanticError(line, col,
            "Model " + modelName + " has no feature schema.");
        return {};
    }

    // generator

    pythonCode
        << "X_predict = " << dataset
        << "[" << modelMeta.trainedFeatureColumnsVariable << "]\n";

    pythonCode
        << output << " = "
        << modelName << ".predict(X_predict)\n";

    pythonCode
        << "print(" << output << ")\n";

    // Symbol table adding
    VariableMetadata predMeta;
    predMeta.type = mls::VariableType::SERIES;

    symbolTable.addVariable(
        output,
        predMeta,
        line,
        col
    );

    return {};


}