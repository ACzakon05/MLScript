#include "PythonGenerator.h"
#include "CustomErrorListener.h"

std::any PythonGenerator::visitTrainStat(MLScriptParser::TrainStatContext *ctx) {
    std::string modelName = std::any_cast<std::string>(ctx->modelName->getText());
    std::string trainSet = std::any_cast<std::string>(ctx->trainSet->getText());

    pythonCode << modelName << ".fit(" << trainSet << "_X, " << trainSet << "_y)\n";

    return {};
}