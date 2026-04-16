#include "PythonGenerator.h"
#include "model/modelDefinition/LinRegDefinition.h"

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

    std::any fitInterceptParam = ctx->FIT_INTERCEPT();
    std::any tolParam = ctx->TOL();
    std::any nJobsParam = ctx->N_JOBS();
    std::any positiveParam = ctx->POSITIVE();

    if (ctx->FIT_INTERCEPT() != nullptr) {
        std::string fitInterceptUser = ctx->fitInterceptVal->getText();

        if (!fitInterceptUser.empty()) {
            std::transform(fitInterceptUser.begin(), fitInterceptUser.end(), fitInterceptUser.begin(), [](unsigned char c){ return std::tolower(c); });
            linReg->params["fitintercept"] = fitInterceptUser == "true" ? "True" : "False";
        }
    }

    if (ctx->TOL() != nullptr) {
        std::string tolUser = ctx->tolVal->getText();

        if (!tolUser.empty()) {
            linReg->params["tol"] = tolUser;
        }
    }

    if (ctx->N_JOBS() != nullptr) {
        std::string nJobsUser = ctx->nJobsVal->getText();

        if (!nJobsUser.empty()) {
            linReg->params["njobs"] = nJobsUser;
        }
    }

    if (ctx->POSITIVE() != nullptr) {
        std::string positiveUser = ctx->positiveVal->getText();

        if (!positiveUser.empty()) {
            std::transform(positiveUser.begin(), positiveUser.end(), positiveUser.begin(), [](unsigned char c){ return std::tolower(c); });
            linReg->params["positive"] = positiveUser == "true" ? "True" : "False";
        }
    }

    return modelDefinition;
}