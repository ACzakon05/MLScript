#pragma once
#include "MLScriptBaseVisitor.h"
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
    std::stringstream pythonCode;

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

private:
    /**
     * @brief Stores options to use when loading files via LOAD command.
     */
    LoadConfig loadConfig;

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
        {"min", "min"}
    };

    /**
     * @brief Retrieves a list of columns in a string format.
     */
    std::string getColumnList(MLScriptParser::ColumnListContext *ctx);

    /**
     * @brief Stores currently processed identifier;
     */
    std::string currentVarName;
};