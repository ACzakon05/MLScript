#pragma once
#include "MLScriptBaseVisitor.h"
#include "CustomErrorListener.h"
#include <sstream>
#include <string>
#include <iostream>

/**
 * @brief Enum with all file extensions available in MLScript.
 */
enum fileExtension {
    CSV,
    SQL,
    JSON,
    PKL
};

/**
 * @brief Stores load options for LOAD command.
 */
struct LoadConfig {
    std::string filePath;
    fileExtension fileFormat = fileExtension::CSV;
    std::string delimiter;
    std::string headerOption;
    std::string columnsToKeep;
    std::string columnsToDiscard;
    std::string nrows;
    std::string orient;
};

class PythonGenerator : public MLScriptBaseVisitor {
public:
    /**
     * @brief Stores the contents of python file header (options, constants, etc.)
     */
    std::stringstream pythonHeader;

    /**
     * @brief Stores the contents of translated python code
     */
    std::stringstream pythonCode;

    PythonGenerator(CustomErrorListener& listener) : diagnostics(listener), isColumnContext(false) {}

    // == Root ==

    /**
     * @brief Entry point for the transpilation process.
     * * This method initializes the Python script by adding essential imports.
     */
    std::any visitProg(MLScriptParser::ProgContext *ctx) override;

    // == Loading Data ==

    /**
     * @brief Transpiles the 'LOAD' statement into a Pandas read_csv call.
     * Examples:
     * * LOAD "data.csv" INTO dataset;
     * * LOAD "data.csv" INTO dataset KEEP "age", "name";
     */
    std::any visitLoadStat(MLScriptParser::LoadStatContext *ctx) override;

    /**
     * @brief Reads general load options.
     */
    std::any visitGeneralLoadOptions(MLScriptParser::GeneralLoadOptionsContext *ctx) override;

    /**
     * @brief Reads load options for CSV files.
     */
    std::any visitLoadCSVFile(MLScriptParser::LoadCSVFileContext *ctx) override;

    /**
     * @brief Reads load options for JSON files.
     */
    std::any visitLoadJSONFile(MLScriptParser::LoadJSONFileContext *ctx) override;

    /**
     * @brief Reads load options for PKL files.
     */
    std::any visitLoadPKLFile(MLScriptParser::LoadPKLFileContext *ctx) override;

    // == Data Inspection and Display ==

    /**
     * @brief
     */
    std::any visitShowStat(MLScriptParser::ShowStatContext *ctx) override;

    /**
     * @brief Displays the entire dataset.
     * Example: SHOW dataset;
     */
    std::any visitShowDataset(MLScriptParser::ShowDatasetContext *ctx) override;

    /**
     * @brief Lists all column names (features) in the dataset.
     * Example: SHOW FEATURES FROM dataset;
     */
    std::any visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) override;

    /**
     * @brief Shows the number of rows or features.
     * Example: SHOW COUNT OF ROWS FROM dataset;
     */
    std::any visitShowCount(MLScriptParser::ShowCountContext *ctx) override;

    /**
     * @brief Displays a specific row by its integer index.
     * Example: SHOW ROW 5 FROM dataset;
     */
    std::any visitShowSingleRow(MLScriptParser::ShowSingleRowContext *ctx) override;

    /**
     * @brief Displays a range of rows.
     * Example: SHOW ROWS 0 TO 10 FROM dataset;
     */
    std::any visitShowMultipleRows(MLScriptParser::ShowMultipleRowsContext *ctx) override;

    /**
     * @brief Displays a single column by name (string) or position (integer).
     * Example: SHOW FEATURE "age" FROM dataset;
     */
    std::any visitShowSingleFeature(MLScriptParser::ShowSingleFeatureContext *ctx) override;

    /**
     * @brief Displays multiple columns by either their names or an integer range.
     * Examples:
     * * SHOW FEATURES "first_name", "age" FROM dataset;
     * * SHOW FEATURES 0 TO 3 FROM dataset;
     */
    std::any visitShowMultipleFeatures(MLScriptParser::ShowMultipleFeaturesContext *ctx) override;

    /**
     * @brief Displays an aggregation function result for given features.
     * Example: SHOW AVG OF "age", "income" FROM dataset;
     */
    std::any visitShowAggFunc(MLScriptParser::ShowAggFuncContext *ctx) override;

    // == Training Preparation ==

    /**
     * @brief Defines which feature in the dataset is the target variable.
     * * Creates a new Python variable `{dataset}_target` to store the label series.
     * Example: SET TARGET "age" FOR dataset; 
     */
    std::any visitSetTargetStat(MLScriptParser::SetTargetStatContext *ctx) override;

    /**
     * @brief Splits a dataset and its target into training and testing subsets.
     * Example: SPLIT my_dataset RATIO 80:20 INTO train_subset, test_subset WITH SEED 42, SHUFFLE true;
     */
    std::any visitSplitStat(MLScriptParser:: SplitStatContext *ctx) override;

    /**
     * @brief Removes rows containing missing values (NA).
     * If column list is provided via ON, only those columns are checked for missing values.
     * Otherwise, all columns are considered.
     * Example: DROP NA FROM my_dataset ON "col1", "col2";
     * Example: DROP NA FROM my_dataset;
     */
    std::any visitDropNaStat(MLScriptParser::DropNaStatContext *ctx) override;

    /**
     * @brief Removes a specific column from a dataset.
     * Example: DROP COLUMN "col1" FROM my_dataset;
     */
    std::any visitDropColumnStat(MLScriptParser::DropColumnStatContext *ctx) override;

    /**
     * @brief Normalizes dataset values to range [0, 1].
     * If ON clause is used, only selected columns are normalized.
     * Otherwise, all numeric columns are processed.
     * Example: NORMALIZE my_dataset ON "col1", "col2";
     * Example: NORMALIZE my_dataset;
     */
    std::any visitNormalizeStat(MLScriptParser::NormalizeStatContext *ctx) override;

/**
 * @brief Standardizes dataset values to mean 0 and standard deviation 1.
 * If ON clause is used, only selected columns are standardized.
 * Otherwise, all numeric columns are processed.
 * Example: STANDARDIZE my_dataset ON "col1", "col2";
 * Example: STANDARDIZE my_dataset;
 */
std::any visitStandardizeStat(MLScriptParser::StandardizeStatContext *ctx) override;



    // == Conditions ==
    
    /**
     * @brief
     */
    std::any visitWhereClause(MLScriptParser::WhereClauseContext *ctx) override;

    /**
     * @brief Visits all inner conditions and outputs them with parenthesis.
     * Example: (condition)
     * @return string
     */
    std::any visitNestedCondition(MLScriptParser::NestedConditionContext *ctx) override;

    /**
     * @brief Visits all inner conditions and outputs their negation.
     * Example: NOT condition
     * @return string
     */
    std::any visitNotCondition(MLScriptParser::NotConditionContext *ctx) override;

    /**
     * @brief Visits all inner conditions and outputs them with relational operators.
     * Example: expression >= expression
     * @return string
     */
    std::any visitRelationalCondition(MLScriptParser::RelationalConditionContext *ctx) override;

    /**
     * @brief Visits all inner conditions and outputs them with logical operators.
     * Example: condition AND condition
     * @return string
     */
    std::any visitLogicalCondition(MLScriptParser::LogicalConditionContext *ctx) override;

    /**
     * @brief Retrieves a column name and outputs a valid pandas column reference.
     * @return string
     */
    std::any visitColumnReference(MLScriptParser::ColumnReferenceContext *ctx) override;

    /**
     * @brief Retrieves a literal value.
     * @return string
     */
    std::any visitLiteralValue(MLScriptParser::LiteralValueContext *ctx) override;

    /**
     * @brief Retrieves a literal.
     * @return string
     */
    std::any visitLiteral(MLScriptParser::LiteralContext *ctx) override;

    /**
     * @brief Visits NAME expression.
     * @return string
     */
    std::any visitNameExpr(MLScriptParser::NameExprContext *ctx) override;

    /**
     * @brief Visits TYPE expression.
     * @return string
     */
    std::any visitTypeExpr(MLScriptParser::TypeExprContext *ctx) override;

    /**
     * @brief Visits MISSING_RATE expression.
     * @return string
     */
    std::any visitMissingRateExpr(MLScriptParser::MissingRateExprContext *ctx) override;

    // == Creating a model ==

    /**
     * @brief
     */
    std::any visitCreateModelStat(MLScriptParser::CreateModelStatContext *ctx) override;

    // LinearRegression

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

    // Ridge

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
    
    // SVC

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

    // == Training ==

    /**
     * @brief Visits the root of the TRAIN statement.
     */
    std::any visitTrainStat(MLScriptParser::TrainStatContext *ctx) override;

private:
    /**
     * 
     */
    CustomErrorListener& diagnostics;
    bool isColumnContext;
    std::unordered_map<std::string, std::string> targetColumns;

    /**
     * @brief Stores options to use when loading files via LOAD command.
     */
    LoadConfig loadConfig;

    /**
     * @brief Stores currently processed identifier;
     */
    std::string currentVarName;

    /**
     * @brief Maps lowercase file formats from grammar to pandas equivalent.
     */
    std::unordered_map<std::string, std::string> fileFormatMap = {
        {"csv", "csv"},
        {"sql", "sql_table"},
        {"pkl", "pickle"},
        {"json", "json"},
        {"html", "html"}
    };

    /**
     * @brief Maps lowercase aggregation function keywords from grammar to pandas functions.
     */
    std::unordered_map<std::string, std::string> aggFuncMap = {
        {"mean", "mean"},
        {"max", "max"},
        {"min", "min"},
        {"sum", "sum"},
        {"median", "median"},
        {"prod", "prod"},
        {"std", "std"},
        {"var", "var"},
        {"unique_vals", "unique"},
        {"unique_count", "nunique"}
    };

    /**
     * @brief Maps lowercase model names from grammar to Scikit equivalent.
     */
    std::unordered_map<std::string, std::string> modelNameMap = {
        {"linear_regression", "LinearRegression"},
        {"naive_bayes", "MultinomialNB"}
    };

    /**
     * @brief Retrieves a list of columns in a string format.
     */
    std::string getColumnList(MLScriptParser::ColumnListContext *ctx);

    /**
     * @brief Returns a .where Pandas statement if applicable;
     */
    std::string applyWhereConditions(MLScriptParser::WhereClauseContext *ctx);

    /**
     * @brief Returns a list comprehension for filtering columns based on conditions.
     */
    std::string applyColumnConditions(MLScriptParser::WhereClauseContext *ctx, const std::string& dataSet);
};