#include "generators/models/ModelGenerator.h"
#include "model/modelDefinition/RidgeDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any ModelGenerator::visitCreateModelRidge(MLScriptParser::CreateModelRidgeContext *ctx) {
    if (ctx->ridgeParamsList() != nullptr) {
        return visit(ctx->ridgeParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyRidge = std::make_shared<RidgeDefinition>();

    return emptyRidge;
}

std::any ModelGenerator::visitRidgeParamsList(MLScriptParser::RidgeParamsListContext *ctx) {
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

std::any ModelGenerator::visitRidgeParamAlpha(MLScriptParser::RidgeParamAlphaContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("alpha", ctx);
}

std::any ModelGenerator::visitRidgeParamFitIntercept(MLScriptParser::RidgeParamFitInterceptContext *ctx) {
    return mlscript::ModelParamParseUtil::parseBooleanParam("fitintercept", ctx);
}

std::any ModelGenerator::visitRidgeParamMaxIter(MLScriptParser::RidgeParamMaxIterContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("maxiter", ctx);
}

std::any ModelGenerator::visitRidgeParamTol(MLScriptParser::RidgeParamTolContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("tol", ctx);
}

std::any ModelGenerator::visitRidgeParamSolver(MLScriptParser::RidgeParamSolverContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("solver", ctx);
}

std::any ModelGenerator::visitRidgeParamPositive(MLScriptParser::RidgeParamPositiveContext *ctx) {
    return mlscript::ModelParamParseUtil::parseBooleanParam("positive", ctx);
}
