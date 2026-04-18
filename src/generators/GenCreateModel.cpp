#include "PythonGenerator.h"
#include "model/modelDefinition/LinRegDefinition.h"
#include "model/modelDefinition/SVCDefinition.h"
#include "model/modelDefinition/RidgeDefinition.h"
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

// == LinearRegression ==

std::any PythonGenerator::visitCreateModelLinReg(MLScriptParser::CreateModelLinRegContext *ctx) {
    pythonHeader << "from sklearn.linear_model import LinearRegression\n";

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
    return ModelParamParseUtil::parseAlNumParam("tol", ctx);
}

std::any PythonGenerator::visitLinRegParamNJobs(MLScriptParser::LinRegParamNJobsContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("njobs", ctx);
}

std::any PythonGenerator::visitLinRegParamPositive(MLScriptParser::LinRegParamPositiveContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("positive", ctx);
}

// == Ridge ==

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

// == SVC ==

std::any PythonGenerator::visitCreateModelSVC(MLScriptParser::CreateModelSVCContext *ctx) {
    pythonHeader << "from sklearn.svm import SVC\n";

    if (ctx->svcParamsList() != nullptr) {
        return visit(ctx->svcParamsList());
    }

    std::shared_ptr<ModelDefinition> emptySVC = std::make_shared<SVCDefinition>();

    return emptySVC;
}

std::any PythonGenerator::visitSvcParamsList(MLScriptParser::SvcParamsListContext *ctx) {
    std::shared_ptr<ModelDefinition> modelDefinition = std::make_shared<SVCDefinition>();

    auto svc = std::static_pointer_cast<SVCDefinition>(modelDefinition);

    for (auto paramCtx : ctx->svcParamWithVal()) {
        auto userParam = visit(paramCtx);

        if (userParam.has_value()) {
            auto userParamPair = std::any_cast<std::pair<std::string, std::string>>(userParam);

            svc->params[userParamPair.first] = userParamPair.second;
        }
    }

    return modelDefinition;
}

std::any PythonGenerator::visitSvcParamC(MLScriptParser::SvcParamCContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("c", ctx);
}

std::any PythonGenerator::visitSvcParamKernel(MLScriptParser::SvcParamKernelContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("kernel", ctx);
}

std::any PythonGenerator::visitSvcParamDegree(MLScriptParser::SvcParamDegreeContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("degree", ctx);
}

std::any PythonGenerator::visitSvcParamGamma(MLScriptParser::SvcParamGammaContext *ctx) {
    return ModelParamParseUtil::parseAlNumParam("gamma", ctx);
}

std::any PythonGenerator::visitSvcParamCoef0(MLScriptParser::SvcParamCoef0Context *ctx) {
    return ModelParamParseUtil::parseAlNumParam("coef0", ctx);
}

std::any PythonGenerator::visitSvcParamProbability(MLScriptParser::SvcParamProbabilityContext *ctx) {
    return ModelParamParseUtil::parseBooleanParam("probability", ctx);
}