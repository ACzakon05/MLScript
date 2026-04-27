#include "generators/models/ModelGenerator.h"
#include "model/modelDefinition/KNNRegDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any ModelGenerator::visitCreateModelKNNReg(MLScriptParser::CreateModelKNNRegContext *ctx) {
    if (ctx->knnParamsList() != nullptr) {
        return visit(ctx->knnParamsList());
    }

    std::shared_ptr<ModelDefinition> emptyKNNReg = std::make_shared<KNNRegDefinition>();

    return emptyKNNReg;
}

std::any ModelGenerator::visitKnnParamsList(MLScriptParser::KnnParamsListContext *ctx) {
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

std::any ModelGenerator::visitKNNRegParamNNeighbors(MLScriptParser::KNNRegParamNNeighborsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("nneighbors", ctx);
}

std::any ModelGenerator::visitKNNRegParamWeights(MLScriptParser::KNNRegParamWeightsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("weights", ctx);
}

std::any ModelGenerator::visitKNNParamAlgo(MLScriptParser::KNNParamAlgoContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("algorithm", ctx);
}

std::any ModelGenerator::visitKNNParamNJobs(MLScriptParser::KNNParamNJobsContext *ctx) {
    return mlscript::ModelParamParseUtil::parseAlNumParam("njobs", ctx);
}