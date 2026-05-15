#include "PythonGenerator.h"
#include "utils/ModelParamParseUtil.h"

#include "semantics/SymbolTable.h"

#include <unordered_map>

std::any PythonGenerator::visitExportModelStat(MLScriptParser::ExportModelStatContext *ctx) {
    std::string modelName = ctx->modelName->getText();
    std::string fileName = ctx->exportName->getText();
    std::string fileFormat = "joblib";

    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();

    if (!symbolTable.exists(modelName)) {
        diagnostics.reportSemanticError(line, col, "Cannot export an uninitialized model " + modelName + ".");
    }

    mls::VariableType modelVarType = symbolTable.get(modelName).type;
    if (modelVarType != mls::VariableType::MODEL) {
        diagnostics.reportSemanticWarning(line, col, "Trying to export object " + modelName + " as model of type " + mls::to_string(modelVarType) + " (expected MODEL).");
    }

    if (ctx->exportParam()) {
        auto fileFormatAny = visit(ctx->exportParam());
        
        if (fileFormatAny.has_value()) {
            fileFormat = std::any_cast<std::string>(fileFormatAny);
        }
    }

    fileName.erase(
        std::remove(fileName.begin(), fileName.end(), '\''),
        fileName.end()
    );

    fileName = "'" +  fileName + "." + fileFormat + "'";

    if (fileFormat == "joblib") {
        pythonHeader << "import joblib\n";

        pythonCode << "joblib.dump(" + modelName + ", " + fileName + ")\n";
    } else if (fileFormat == "pkl") {
        pythonHeader << "import pickle\n";

        pythonCode << "with open(" + fileName + ", 'wb') as file:\n";
        pythonCode << "\tpickle.dump(" + modelName + ", file)\n";
    }

    return {};
}

std::any PythonGenerator::visitExportModelFormatParam(MLScriptParser::ExportModelFormatParamContext *ctx) {
    auto paramPair = mlscript::ModelParamParseUtil::parseAlNumParam("format", ctx);
    std::string fileFormat = paramPair.second;
    std::transform(
        fileFormat.begin(),
        fileFormat.end(),
        fileFormat.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

    return fileFormat;
}