#include "PythonGenerator.h"
#include "model/modelDefinition/SVCDefinition.h"
#include "utils/ModelParamParseUtil.h"

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
    return mlscript::ModelParamParseUtil::parseAlNumParam("c", ctx);
}

std::any PythonGenerator::visitSvcParamKernel(MLScriptParser::SvcParamKernelContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("kernel", ctx);
}

std::any PythonGenerator::visitSvcParamDegree(MLScriptParser::SvcParamDegreeContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("degree", ctx);
}

std::any PythonGenerator::visitSvcParamGamma(MLScriptParser::SvcParamGammaContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("gamma", ctx);
}

std::any PythonGenerator::visitSvcParamCoef0(MLScriptParser::SvcParamCoef0Context *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("coef0", ctx);
}

std::any PythonGenerator::visitSvcParamProbability(MLScriptParser::SvcParamProbabilityContext *ctx) {
    return mlscript::ModelParamParseUtil::parseBooleanParam("probability", ctx);
}