#include "generators/models/ModelGenerator.h"
#include "model/modelDefinition/LinRegDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any ModelGenerator::visitCreateModelLinReg(MLScriptParser::CreateModelLinRegContext *ctx) {
    if (ctx->linearRegressionParamsList() != nullptr) {
        return visit(ctx->linearRegressionParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyLinReg = std::make_shared<LinRegDefinition>();

    return emptyLinReg;
}

std::any ModelGenerator::visitLinearRegressionParamsList(MLScriptParser::LinearRegressionParamsListContext *ctx) {
    std::shared_ptr<ModelDefinition> modelDefinition = std::make_shared<LinRegDefinition>();

    auto linReg = std::static_pointer_cast<LinRegDefinition>(modelDefinition);

    for (auto paramCtx : ctx->linRegModelParamWithVal()) {
        auto userParam = visit(paramCtx);

        if (userParam.has_value()) {
            auto userParamPair = std::any_cast<std::pair<std::string, std::string>>(userParam);

            linReg->params[userParamPair.first] = userParamPair.second;
        }
    }

    return modelDefinition;
}

std::any ModelGenerator::visitLinRegParamFitIntercept(MLScriptParser::LinRegParamFitInterceptContext *ctx) {
    return mlscript::ModelParamParseUtil::parseBooleanParam("fitintercept", ctx);
}

std::any ModelGenerator::visitLinRegParamTol(MLScriptParser::LinRegParamTolContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("tol", ctx);
}

std::any ModelGenerator::visitLinRegParamNJobs(MLScriptParser::LinRegParamNJobsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("njobs", ctx);
}

std::any ModelGenerator::visitLinRegParamPositive(MLScriptParser::LinRegParamPositiveContext *ctx) {
    return mlscript::ModelParamParseUtil::parseBooleanParam("positive", ctx);
}