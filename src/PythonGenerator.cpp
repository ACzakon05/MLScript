#include "PythonGenerator.h"

// == Root ==

std::any PythonGenerator::visitProg(MLScriptParser::ProgContext *ctx) {
    pythonCode << "import pandas as pd\n";

    return visitChildren(ctx);
}

// == Loading Data ==

std::any PythonGenerator::visitLoadStat(MLScriptParser::LoadStatContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string filePath = ctx->STRING()->getText();
    std::stringstream loadOptions;

    visit(ctx->fileFormatLoadOptions());

    pythonCode << varName << " = " << "pd.read_";

    switch (loadConfig.fileFormat)
    {
    case fileExtension::CSV:
        loadOptions << "csv(\n";
        loadOptions << "\tfilepath_or_buffer=" << filePath << ",\n";
        loadOptions << "\tdelimiter=\"" << loadConfig.delimiter << "\",\n";
        loadOptions << "\theader=\"" << loadConfig.headerOption << "\"\n";
        loadOptions << ")\n";
        break;
    case fileExtension::JSON:
        loadOptions << "json(\n";
        loadOptions << "\tpath_or_buf=" << filePath << ",\n";
        loadOptions << "\torient=" << loadConfig.orient << "\n";
        loadOptions << ")\n";
    default:
        break;
    }

    pythonCode << loadOptions.str();

    visit(ctx->generalLoadOptions());

    if (!loadConfig.columnsToKeep.empty()) {
        pythonCode << varName << " = " << varName << "[[" << loadConfig.columnsToKeep << "]]\n";
    }
    if (!loadConfig.columnsToDiscard.empty()) {
        pythonCode << varName << " = " << varName << ".drop(columns=[" << loadConfig.columnsToDiscard << "])\n";
    }
    if (!loadConfig.nrows.empty()) {
        pythonCode << varName << " = " << varName << ".iloc[:" << loadConfig.nrows << "]\n";
    }
    
    return {};
}

std::any PythonGenerator::visitGeneralLoadOptions(MLScriptParser::GeneralLoadOptionsContext *ctx) {
    int columnListIndex = 0;

    if (ctx->KEEP()) {
        loadConfig.columnsToKeep = getColumnList(ctx->columnList(columnListIndex));
        columnListIndex++;
    }
    if (ctx->WITHOUT()) {
        loadConfig.columnsToDiscard = getColumnList(ctx->columnList(columnListIndex));
        columnListIndex++;
    }
    if (ctx->LIMIT()) {
        loadConfig.nrows = ctx->INTEGER()->getText();
    }

    return {};
}

std::any PythonGenerator::visitLoadCSVFile(MLScriptParser::LoadCSVFileContext *ctx) {
    std::string delimiter = ",";
    std::string keepHeader = "infer";

    if (ctx->csvLoadOptions()->DELIMITED()) {
        std::string rawDelimiter = ctx->csvLoadOptions()->STRING()->getText();

        if (rawDelimiter.length() >= 2 &&
           (rawDelimiter.front() == '\'' || rawDelimiter.front() == '"')) 
        {
            delimiter = rawDelimiter.substr(1, rawDelimiter.length() - 2);
        } 
        else 
        {
            delimiter = ctx->csvLoadOptions()->STRING()->getText();
        }
    }
    if (ctx->csvLoadOptions()->HEADER()) {
        if (ctx->csvLoadOptions()->WITHOUT()) {
            keepHeader = "1";
        }
    }

    loadConfig.delimiter = delimiter;
    loadConfig.headerOption = keepHeader;
    loadConfig.fileFormat = fileExtension::CSV;

    return {};
}

std::any PythonGenerator::visitLoadJSONFile(MLScriptParser::LoadJSONFileContext *ctx) {
    std::string orient = "None";

    if (ctx->jsonLoadOptions()->ORIENT()) {
        orient = ctx->jsonLoadOptions()->STRING()->getText();
    }

    loadConfig.orient = orient;
    loadConfig.fileFormat = fileExtension::JSON;

    return {};
}

// == Data Inspection and Display ==

std::any PythonGenerator::visitShowDataset(MLScriptParser::ShowDatasetContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    pythonCode << "print(" << varName << ")\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    pythonCode << "print(f'Columns in " << varName << ": {" << varName << ".columns.tolist()}')\n"; 

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowCount(MLScriptParser::ShowCountContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->FEATURES()) {
        pythonCode << "print(f'Numer of features in " << varName << ": {len(" << varName << ".columns)}')\n";
    } 
    else if (ctx->ROWS()) {
        pythonCode << "print(f'Numer of rows in " << varName << ": {len(" << varName << ")}')\n";
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowSingleRow(MLScriptParser::ShowSingleRowContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string rowNumber = ctx->INTEGER()->getText();

    pythonCode << "print('" << varName << " row at position " << rowNumber << ":')\n";
    pythonCode << "print(f'{" << varName << ".iloc[" << rowNumber << "]}')\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowMultipleRows(MLScriptParser::ShowMultipleRowsContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string lowerBound = ctx->INTEGER(0)->getText();
    std::string upperBound = std::to_string(std::stoi(ctx->INTEGER(1)->getText()) + 1);

    pythonCode << "print('" << varName << " rows from position " << lowerBound << " to position " << upperBound << ":')\n";
    pythonCode << "print(f'{" << varName << ".iloc[" << lowerBound << ":" << upperBound << "]}')\n";

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowSingleFeature(MLScriptParser::ShowSingleFeatureContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->STRING()) {
        std::string featureName = ctx->STRING()->getText();
        pythonCode << "print(" << varName << "[" << featureName << "])\n";
    } 
    else if (ctx->INTEGER()) 
    {
        int featureIndex = std::stoi(ctx->INTEGER()->getText()) ;
        pythonCode << "print(" << varName << ".iloc[: , " << featureIndex - 1 << "])\n";
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowMultipleFeatures(MLScriptParser::ShowMultipleFeaturesContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();

    if (ctx->columnList()) {
        pythonCode << "print(" << varName << "[[" << getColumnList(ctx->columnList()) << "]])\n";
    } 
    else if (ctx->INTEGER(0) && ctx->INTEGER(1))
    {
        std::string lowerBound = ctx->INTEGER(0)->getText();
        std::string upperBound = ctx->INTEGER(1)->getText();

        pythonCode << "print(" << varName << ".iloc[:, " << lowerBound << ":" << upperBound << "])\n"; 
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitShowAggFunc(MLScriptParser::ShowAggFuncContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string aggFunc = ctx->aggFunc()->getText();
    std::transform(aggFunc.begin(), aggFunc.end(), aggFunc.begin(), ::tolower);

    pythonCode << "print(" << varName << "[[" << getColumnList(ctx->columnList()) << "]]";
    pythonCode << ".agg(['" << aggFuncMap.at(aggFunc) << "']))\n";

    return visitChildren(ctx);
}

// == Training Preparation ==

std::any PythonGenerator::visitSetTargetStat(MLScriptParser::SetTargetStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER()->getText();
    std::string targetCol = ctx->STRING()->getText();
    pythonCode<< "#Set target column for " << dataSet << ": " << targetCol << "\n";
    pythonCode << dataSet << "_target = " << dataSet << "[" << targetCol << "]\n";
    
    return visitChildren(ctx);
} 

//SPLIT my_dataset RATIO 80:20 INTO train_subset, test_subset WITH SEED 42, SHUFFLE true;
std::any PythonGenerator::visitSplitStat(MLScriptParser:: SplitStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER(0)->getText();
    std::string trainSubset = ctx->IDENTIFIER(1)->getText();
    std::string testSubset = ctx->IDENTIFIER(2)->getText();

    std::string ratioText = ctx->RATIO()->getText();
    size_t colonPos = ratioText.find(':');
    std::string trainRatio = ratioText.substr(0, colonPos);
    std::string testRatio = ratioText.substr(colonPos + 1);
    double testSize = std::stod(testRatio) / (std::stod(trainRatio) + std::stod(testRatio));

    // Opcjonalne seed ii shufle

    int seed =0;
    std::string shuffle = "True";
    if (ctx->SEED() && ctx->INTEGER()) {
        seed = std::stoi(ctx->INTEGER()->getText());
    }

    // obsługa TRUE / FALSE
    if (ctx->TRUE()) {
        shuffle = "True";
    } else if (ctx->FALSE()) {
        shuffle = "False";
    }
    
    pythonCode << "# Split dataset into train and test\n";
    pythonCode << "from sklearn.model_selection import train_test_split\n";
    pythonCode << trainSubset << ", " << testSubset << ", " 
            << trainSubset << "_target, " << testSubset << "_target = train_test_split(\n";
    pythonCode << "    " << dataSet << ".drop(columns=[" << dataSet << "_target.name]),\n";
    pythonCode << "    " << dataSet << "_target,\n";
    pythonCode << "    test_size=" << testSize << ",\n";
    pythonCode << "    random_state=" << seed << ",\n";
    pythonCode << "    shuffle=" << shuffle << "\n";
    pythonCode << ")\n";

    return visitChildren(ctx);
}