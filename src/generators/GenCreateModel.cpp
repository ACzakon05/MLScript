#include "PythonGenerator.h"
#include "model/modelDefinition/LinRegDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any PythonGenerator::visitCreateModelStat(MLScriptParser::CreateModelStatContext *ctx) {
    std::string modelName = ctx->IDENTIFIER()->getText();

    pythonCode << modelName << " = ";

    std::any modelDefinitionAny = visit(ctx->modelDefinition());
    
    try {
        auto modelDefinition = std::any_cast<std::shared_ptr<ModelDefinition>>(modelDefinitionAny);

        modelDefinition->toScikit(pythonCode);
    } catch (const std::bad_any_cast& e) {
        size_t line = ctx->getStart()->getLine();
        size_t col = ctx->getStart()->getCharPositionInLine();

        diagnostics.reportSemanticError(line, col, "Expected a ModelDefinition in Model generation");
    }

    return {};
}

std::any PythonGenerator::visitCreateModelLinReg(MLScriptParser::CreateModelLinRegContext *ctx) {
    if (ctx->linearRegressionParamsList() != nullptr) {
        return visit(ctx->linearRegressionParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyLinReg = std::make_shared<LinRegDefinition>();

    return emptyLinReg;
}

std::any PythonGenerator::visitLinearRegressionParamsList(MLScriptParser::LinearRegressionParamsListContext *ctx) {
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

std::any PythonGenerator::visitLinRegParamFitIntercept(MLScriptParser::LinRegParamFitInterceptContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("fitintercept", ctx);
}

std::any PythonGenerator::visitLinRegParamTol(MLScriptParser::LinRegParamTolContext *ctx) {
    return ModelParamParseUtil::parseNumberParam("tol", ctx);
}

std::any PythonGenerator::visitLinRegParamNJobs(MLScriptParser::LinRegParamNJobsContext *ctx) {
    return ModelParamParseUtil::parseNumberParam("njobs", ctx);
}

std::any PythonGenerator::visitLinRegParamPositive(MLScriptParser::LinRegParamPositiveContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("positive", ctx);
}