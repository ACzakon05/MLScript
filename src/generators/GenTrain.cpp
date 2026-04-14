#include "PythonGenerator.h"
#include "CustomErrorListener.h"

std::any PythonGenerator::visitTrainStat(MLScriptParser::TrainStatContext *ctx) {
    std::string modelName = ctx->IDENTIFIER(0)->getText();
    std::string varName = ctx->IDENTIFIER(1)->getText();

    std::string model = std::any_cast<std::string>(visit(ctx->trainOption()));

    pythonCode << modelName << " = " << model << "()\n";

    return {};
}

std::any PythonGenerator::visitTrainOption(MLScriptParser::TrainOptionContext *ctx) {
    std::string model = std::any_cast<std::string>(visit(ctx->children[0]));
    std::transform(model.begin(), model.end(), model.begin(), [](unsigned char c){ return std::tolower(c); });

    return modelNameMap.at(model);
}

std::any PythonGenerator::visitRegressor(MLScriptParser::RegressorContext *ctx) {
    return ctx->getText();
}

std::any PythonGenerator::visitClassifier(MLScriptParser::ClassifierContext *ctx) {
    return ctx->getText();
}