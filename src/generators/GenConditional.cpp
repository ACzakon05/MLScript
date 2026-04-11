#include "PythonGenerator.h"

std::any PythonGenerator::visitNotCondition(MLScriptParser::NotConditionContext *ctx) {
    std::string inner = std::any_cast<std::string>(visit(ctx->condition()));

    return std::string("~(" + inner + ")");
}