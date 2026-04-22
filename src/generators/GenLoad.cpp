#include "PythonGenerator.h"

#include "semantics/SymbolTable.h"

std::any PythonGenerator::visitLoadStat(MLScriptParser::LoadStatContext *ctx) {
    std::string varName = ctx->IDENTIFIER()->getText();
    std::string filePath = ctx->STRING()->getText();
    std::stringstream loadOptions;
    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();

    symbolTable.addVariable(
        varName,
        VariableMetadata{mls::VariableType::DATASET, varName},
        line, col
    );

    std::string rawFilePath = filePath;
    if (rawFilePath.length() >= 2 && rawFilePath.front() == '\'') {
        rawFilePath = rawFilePath.substr(1, rawFilePath.length() - 2);
    }

    std::ifstream testFile(rawFilePath);
    if (!testFile.good()) {
        size_t line = ctx->getStart()->getLine();
        size_t col = ctx->getStart()->getCharPositionInLine();

        diagnostics.reportSemanticWarning(line, col, "File " + filePath + "not found at compile time.");
    }

    visit(ctx->fileFormatLoadOptions());

    pythonCode << "with safe_execute_load(f\"loading " << filePath << "\"):\n";
    pythonCode << "\t" << varName << " = " << "pd.read_";

    switch (loadConfig.fileFormat)
    {
    case mlscript::fileExtension::CSV:
        loadOptions << "csv(\n";
        loadOptions << "\t\tfilepath_or_buffer=" << filePath << ",\n";
        loadOptions << "\t\tdelimiter=\"" << loadConfig.delimiter << "\",\n";
        loadOptions << "\t\theader=\"" << loadConfig.headerOption << "\"\n";
        loadOptions << "\t)\n";
        break;
    case mlscript::fileExtension::JSON:
        loadOptions << "json(\n";
        loadOptions << "\t\tpath_or_buf=" << filePath << ",\n";
        loadOptions << "\t\torient=" << loadConfig.orient << "\n";
        loadOptions << "\t)\n";
        break;
    case mlscript::fileExtension::PKL:
        loadOptions << "pickle(\n";
        loadOptions << "\t\tfilepath_or_buffer=" << filePath << "\n";
        loadOptions << "\t)\n";
        break;
    default:
        break;
    }

    pythonCode << "" << loadOptions.str();

    visit(ctx->generalLoadOptions());

    if (!loadConfig.columnsToKeep.empty()) {
        pythonCode << "\t" << varName << " = " << varName << "[[" << loadConfig.columnsToKeep << "]]\n";
    }
    if (!loadConfig.columnsToDiscard.empty()) {
        pythonCode << "\t" << varName << " = " << varName << ".drop(columns=[" << loadConfig.columnsToDiscard << "])\n";
    }
    if (!loadConfig.nrows.empty()) {
        pythonCode << "\t" << varName << " = " << varName << ".iloc[:" << loadConfig.nrows << "]\n";
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
    loadConfig.fileFormat = mlscript::fileExtension::CSV;

    return {};
}

std::any PythonGenerator::visitLoadJSONFile(MLScriptParser::LoadJSONFileContext *ctx) {
    std::string orient = "None";

    if (ctx->jsonLoadOptions()->ORIENT()) {
        orient = ctx->jsonLoadOptions()->STRING()->getText();
    }

    loadConfig.orient = orient;
    loadConfig.fileFormat = mlscript::fileExtension::JSON;

    return {};
}

std::any PythonGenerator::visitLoadPKLFile(MLScriptParser::LoadPKLFileContext *ctx) {
    loadConfig.fileFormat = mlscript::fileExtension::PKL;

    return {};
}