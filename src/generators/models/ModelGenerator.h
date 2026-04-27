#pragma once
#include "MLScriptBaseVisitor.h"
#include "CustomErrorListener.h"
#include <string>
#include <any>

class ModelGenerator : public MLScriptBaseVisitor {
private:
    CustomErrorListener& diagnostics;

public:
    ModelGenerator(CustomErrorListener& listener) : diagnostics(listener) {}

    // =================
    // Regressors
    // =================

    // -----------------
    // Linear Regression
    // -----------------

    /**
     * @brief
     */
    std::any visitCreateModelLinReg(MLScriptParser::CreateModelLinRegContext *ctx) override;

    /**
     * @brief
     */
    std::any visitLinearRegressionParamsList(MLScriptParser::LinearRegressionParamsListContext *ctx) override;

    /**
     * @brief
     */
    std::any visitLinRegParamFitIntercept(MLScriptParser::LinRegParamFitInterceptContext *ctx) override;

    /**
     * @brief
     */
    std::any visitLinRegParamTol(MLScriptParser::LinRegParamTolContext *ctx) override;

    /**
     * @brief
     */
    std::any visitLinRegParamNJobs(MLScriptParser::LinRegParamNJobsContext *ctx) override;

    /**
     * @brief
     */
    std::any visitLinRegParamPositive(MLScriptParser::LinRegParamPositiveContext *ctx) override;

    // -----------------
    // Ridge
    // -----------------

    /**
     * @brief
     */
    std::any visitCreateModelRidge(MLScriptParser::CreateModelRidgeContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamsList(MLScriptParser::RidgeParamsListContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamAlpha(MLScriptParser::RidgeParamAlphaContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamFitIntercept(MLScriptParser::RidgeParamFitInterceptContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamMaxIter(MLScriptParser::RidgeParamMaxIterContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamTol(MLScriptParser::RidgeParamTolContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamSolver(MLScriptParser::RidgeParamSolverContext *ctx) override;

    /**
     * @brief
     */
    std::any visitRidgeParamPositive(MLScriptParser::RidgeParamPositiveContext *ctx) override;

    // -----------------
    // KNN Regressor
    // -----------------

    /**
     * @brief
     */
    std::any visitCreateModelKNNReg(MLScriptParser::CreateModelKNNRegContext *ctx) override;

    /**
     * @brief
     */
    std::any visitKnnParamsList(MLScriptParser::KnnParamsListContext *ctx) override;

    /**
     * @brief
     */
    std::any visitKNNRegParamNNeighbors(MLScriptParser::KNNRegParamNNeighborsContext *ctx) override;

    /**
     * @brief
     */
    std::any visitKNNRegParamWeights(MLScriptParser::KNNRegParamWeightsContext *ctx) override;

    /**
     * @brief
     */
    std::any visitKNNParamAlgo(MLScriptParser::KNNParamAlgoContext *ctx) override;

    /**
     * @brief
     */
    std::any visitKNNParamNJobs(MLScriptParser::KNNParamNJobsContext *ctx) override;

    
    // =================
    // Classifiers
    // =================

    // -----------------
    // SVC
    // -----------------

    /**
     * @brief
     */
    std::any visitCreateModelSVC(MLScriptParser::CreateModelSVCContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamsList(MLScriptParser::SvcParamsListContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamC(MLScriptParser::SvcParamCContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamKernel(MLScriptParser::SvcParamKernelContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamDegree(MLScriptParser::SvcParamDegreeContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamGamma(MLScriptParser::SvcParamGammaContext *ctx) override;

    /**
     * @brief
     */
    std::any visitSvcParamCoef0(MLScriptParser::SvcParamCoef0Context *ctx) override;

    /**
     * @brief
     */
     std::any visitSvcParamProbability(MLScriptParser::SvcParamProbabilityContext *ctx) override;
};