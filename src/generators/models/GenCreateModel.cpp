#include "PythonGenerator.h"
#include "model/modelDefinition/ModelDefinition.h"
#include "utils/ModelParamParseUtil.h"

std::any PythonGenerator::visitCreateModelStat(MLScriptParser::CreateModelStatContext *ctx) {
    std::string modelName = ctx->IDENTIFIER()->getText();

    pythonCode << modelName << " = ";

    std::any modelDefinitionAny = visit(ctx->modelDefinition());
    
    try {
        auto modelDefinition = std::any_cast<std::shared_ptr<ModelDefinition>>(modelDefinitionAny);

        modelDefinition->toScikit(pythonCode);
    } catch (const std::bad_any_cast& e) {
        size_t line = ctx->getStart()->getLine();
        size_t col = ctx->getStart()->getCharPositionInLine();

        diagnostics.reportSemanticError(line, col, "Expected a ModelDefinition in Model generation");
    }

    return {};
}