#include "PythonGenerator.h"

std::any PythonGenerator::visitWhereClause(MLScriptParser::WhereClauseContext *ctx) {
    std::string conditions = std::any_cast<std::string>(visit(ctx->condition()));

    return conditions;
}

std::any PythonGenerator::visitNestedCondition(MLScriptParser::NestedConditionContext *ctx) {
    std::string innerConditions = std::any_cast<std::string>(visit(ctx->condition()));

    return std::string(" (" + innerConditions + ") ");
}

std::any PythonGenerator::visitNotCondition(MLScriptParser::NotConditionContext *ctx) {
    std::string innerConditions = std::any_cast<std::string>(visit(ctx->condition()));

    return std::string(" ~(" + innerConditions + ") ");
}

std::any PythonGenerator::visitLogicalCondition(MLScriptParser::LogicalConditionContext *ctx) {
    std::string leftCondition = std::any_cast<std::string>(visit(ctx->left));
    std::string rightCondition = std::any_cast<std::string>(visit(ctx->right));

    std::string logicalOperator = ctx->logicalOperator()->getText();
    std::transform(logicalOperator.begin(), logicalOperator.end(), logicalOperator.begin(), [](unsigned char c){ return std::tolower(c); });

    std::string logicalOperatorPandas = logicalOperator == "and" ? "&" : "|";

    return std::string(" (" + leftCondition + " " + logicalOperatorPandas + " " + rightCondition + ") ");
}

std::any PythonGenerator::visitRelationalCondition(MLScriptParser::RelationalConditionContext *ctx) {
std::string leftExpression = std::any_cast<std::string>(visit(ctx->expression(0)));
    std::string rightExpression = std::any_cast<std::string>(visit(ctx->expression(1)));
    std::string relationalOperator = ctx->comparisonOperator()->getText();

    std::string relationalOperatorPandas = relationalOperator == "=" ? "==" : relationalOperator;

    return std::string(" (" + leftExpression + " " + relationalOperatorPandas + " " + rightExpression + ") ");
}

std::any PythonGenerator::visitColumnReference(MLScriptParser::ColumnReferenceContext *ctx) {
    std::string columnName = ctx->COL_NAME()->getText();

    if (isColumnContext) {
        return columnName;  // For column conditions, COL_NAME refers to column name as string
    } else {
        return std::string(currentVarName + "[" + columnName + "]");
    }
}

std::any PythonGenerator::visitLiteralValue(MLScriptParser::LiteralValueContext *ctx) {
    std::string literal = std::any_cast<std::string>(visit(ctx->literal()));

    return literal;
}

std::any PythonGenerator::visitLiteral(MLScriptParser::LiteralContext *ctx) {
    std::string literal = ctx->getText();

    return literal;
}

std::any PythonGenerator::visitNameExpr(MLScriptParser::NameExprContext *ctx) {
    if (isColumnContext) {
        return std::string("col");
    } else {
        return std::string("");
    }
}

std::any PythonGenerator::visitTypeExpr(MLScriptParser::TypeExprContext *ctx) {
    if (isColumnContext) {
        return std::string("DATASET[col].dtype");
    } else {
        return std::string("");
    }
}

std::any PythonGenerator::visitMissingRateExpr(MLScriptParser::MissingRateExprContext *ctx) {
    if (isColumnContext) {
        return std::string("DATASET[col].isna().sum() / len(DATASET)");
    } else {
        return std::string("");
    }
}