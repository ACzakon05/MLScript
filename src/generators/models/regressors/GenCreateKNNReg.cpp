#include "PythonGenerator.h"
#include "model/modelDefinition/KNNRegDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any PythonGenerator::visitCreateModelKNNReg(MLScriptParser::CreateModelKNNRegContext *ctx) {
    pythonHeader << "from sklearn.neighbors import KNeighborsRegressor\n";

    if (ctx->knnParamsList() != nullptr) {
        return visit(ctx->knnParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyKNNReg = std::make_shared<KNNRegDefinition>();

    return emptyKNNReg;
}

std::any PythonGenerator::visitKnnParamsList(MLScriptParser::KnnParamsListContext *ctx) {
    std::shared_ptr<ModelDefinition> modelDefinition = std::make_shared<KNNRegDefinition>();

    auto knnReg = std::static_pointer_cast<KNNRegDefinition>(modelDefinition);

    for (auto paramCtx : ctx->knnModelParamWithVal()) {
        auto userParam = visit(paramCtx);

        if (userParam.has_value()) {
            auto userParamPair = std::any_cast<std::pair<std::string, std::string>>(userParam);

            knnReg->params[userParamPair.first] = userParamPair.second;
        }
    }

    return modelDefinition;
}

std::any PythonGenerator::visitKNNRegParamNNeighbors(MLScriptParser::KNNRegParamNNeighborsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("nneighbors", ctx);
}

std::any PythonGenerator::visitKNNRegParamWeights(MLScriptParser::KNNRegParamWeightsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("weights", ctx);
}

std::any PythonGenerator::visitKNNParamAlgo(MLScriptParser::KNNParamAlgoContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("algorithm", ctx);
}

std::any PythonGenerator::visitKNNParamNJobs(MLScriptParser::KNNParamNJobsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("njobs", ctx);
}