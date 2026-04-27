#include "PythonGenerator.h"
#include "ModelGenerator.h"
#include "model/modelDefinition/ModelDefinition.h"
#include "utils/ModelParamParseUtil.h"
#include "semantics/SymbolTable.h"

std::any PythonGenerator::visitCreateModelStat(MLScriptParser::CreateModelStatContext *ctx) {
    std::string modelName = ctx->IDENTIFIER()->getText();
    size_t line = ctx->getStart()->getLine();
    size_t col = ctx->getStart()->getCharPositionInLine();

    symbolTable.addVariable(
        modelName,
        VariableMetadata{mls::VariableType::MODEL, modelName},
        line, col
    );

    pythonCode << modelName << " = ";

    ModelGenerator modelGen(diagnostics);

    std::any modelDefinitionAny = modelGen.visit(ctx->modelDefinition());
    
    try {
        auto modelDefinition = std::any_cast<std::shared_ptr<ModelDefinition>>(modelDefinitionAny);

        pythonHeader << modelDefinition->getRequiredImport();
        modelDefinition->toScikit(pythonCode);
    } catch (const std::bad_any_cast& e) {
        diagnostics.reportSemanticError(line, col, "Expected a ModelDefinition in Model generation");
    }

    return {};
}