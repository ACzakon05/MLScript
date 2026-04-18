#include "PythonGenerator.h"
#include "model/modelDefinition/RidgeDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any PythonGenerator::visitCreateModelRidge(MLScriptParser::CreateModelRidgeContext *ctx) {
    pythonHeader << "from sklearn.linear_model import Ridge\n";

    if (ctx->ridgeParamsList() != nullptr) {
        return visit(ctx->ridgeParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyRidge = std::make_shared<RidgeDefinition>();

    return emptyRidge;
}

std::any PythonGenerator::visitRidgeParamsList(MLScriptParser::RidgeParamsListContext *ctx) {
    std::shared_ptr<ModelDefinition> modelDefinition = std::make_shared<RidgeDefinition>();

    auto ridge = std::static_pointer_cast<RidgeDefinition>(modelDefinition);

    for (auto paramCtx : ctx->ridgeModelParamWithVal()) {
        auto userParam = visit(paramCtx);

        if (userParam.has_value()) {
            auto userParamPair = std::any_cast<std::pair<std::string, std::string>>(userParam);

            ridge->params[userParamPair.first] = userParamPair.second;
        }
    }

    return modelDefinition;
}

std::any PythonGenerator::visitRidgeParamAlpha(MLScriptParser::RidgeParamAlphaContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("alpha", ctx);
}

std::any PythonGenerator::visitRidgeParamFitIntercept(MLScriptParser::RidgeParamFitInterceptContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("fitintercept", ctx);
}

std::any PythonGenerator::visitRidgeParamMaxIter(MLScriptParser::RidgeParamMaxIterContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("maxiter", ctx);
}

std::any PythonGenerator::visitRidgeParamTol(MLScriptParser::RidgeParamTolContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("tol", ctx);
}

std::any PythonGenerator::visitRidgeParamSolver(MLScriptParser::RidgeParamSolverContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("solver", ctx);
}

std::any PythonGenerator::visitRidgeParamPositive(MLScriptParser::RidgeParamPositiveContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("positive", ctx);
}
