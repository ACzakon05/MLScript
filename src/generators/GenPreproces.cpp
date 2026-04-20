#include "PythonGenerator.h"

std::any PythonGenerator::visitDropNaStat(MLScriptParser::DropNaStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Drop NA from " << dataSet << "\n";
    if (ctx->columnList()){
        pythonCode << dataSet << " = " << dataSet << ".dropna(subset=[" << getColumnList(ctx->columnList()) << "])\n";
    }
    else{
        pythonCode << dataSet << " = " << dataSet << ".dropna()\n";
    }
    return visitChildren(ctx);
}

std::any PythonGenerator::visitDropColumnStat(MLScriptParser::DropColumnStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Drop columns from " << dataSet << "\n";
    pythonCode << getDatasetExistenceCheck(dataSet);

    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        pythonCode << "columns_to_drop = [" << columns << "]\n";
        pythonCode << getColumnsExistenceCheck(dataSet, "columns_to_drop");

        if (ctx->safeMode()) {
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "' if present\n";
                pythonCode << "if '" << target << "' in columns_to_drop: columns_to_drop.remove('" << target << "')\n";
            }
        }

        pythonCode << dataSet << " = " << dataSet << ".drop(columns=columns_to_drop)\n";
    }
    else if (ctx->whereClause()){
        std::string columnFilter = applyColumnConditions(ctx->whereClause(), dataSet);
        pythonCode << "columns_to_drop = " << columnFilter << "\n";
        pythonCode << getColumnsExistenceCheck(dataSet, "columns_to_drop");

        if (ctx->safeMode()) {
            auto it = targetColumns.find(dataSet);
            if (it != targetColumns.end()) {
                std::string target = it->second;
                pythonCode << "# Safe mode: excluding target column '" << target << "'\n";
                pythonCode << "columns_to_drop = [col for col in columns_to_drop if col != '" << target << "']\n";
            }
        }

        pythonCode << dataSet << " = " << dataSet << ".drop(columns=columns_to_drop)\n";
    }
    return visitChildren(ctx);
}

std::any PythonGenerator::visitNormalizeStat(MLScriptParser::NormalizeStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Normalize dataset\n";
    pythonCode << getDatasetExistenceCheck(dataSet);

    std::string method = "min-max";
    std::string rangeType = "0_1";
    std::string handle = "none";

    if (ctx->normalizeOptions()) {
        auto options = ctx->normalizeOptions();
        if (options->methodOption()) {
            auto m = options->methodOption()->methodType();
            if (m->MINMAX()) method = "min-max";
            else if (m->ROBUST()) method = "robust";
        }
        if (options->rangeOption()) {
            auto r = options->rangeOption()->rangeType();
            if (r->ZERO_ONE()) rangeType = "0_1";
            else if (r->MINUS_ONE_ONE()) rangeType = "-1_1";
        }
        if (options->handleOption()) {
            auto h = options->handleOption()->handleType();
            if (h->DROP_NA()) handle = "drop_na";
            else if (h->FILL_MEAN()) handle = "fill_mean";
            else if (h->FILL_MEDIAN()) handle = "fill_median";
        }
    }

    pythonCode << "import numpy as np\n";
    pythonCode << dataSet << " = " << dataSet << ".replace([np.inf, -np.inf], np.nan)\n";

    if (handle == "fill_mean") {
        pythonCode << dataSet << " = " << dataSet << ".apply(lambda col: col.fillna(col.mean()) if np.issubdtype(col.dtype, np.number) else col)\n";
    } else if (handle == "fill_median") {
        pythonCode << dataSet << " = " << dataSet << ".apply(lambda col: col.fillna(col.median()) if np.issubdtype(col.dtype, np.number) else col)\n";
    } else if (handle == "drop_na") {
        pythonCode << dataSet << " = " << dataSet << ".dropna()\n";
    }

    if (ctx->columnList()) {
        std::string columnsToNormalize = getColumnList(ctx->columnList());
        pythonCode << getColumnsExistenceCheck(dataSet, "columns_to_normalize");
        pythonCode << "cols_to_normalize = [" << columnsToNormalize << "]\n";
        if (method == "min-max") {
            if (rangeType == "0_1") {
                pythonCode << dataSet << "[cols_to_normalize] = (" << dataSet << "[cols_to_normalize] - " << dataSet << "[cols_to_normalize].min()) / (" << dataSet << "[cols_to_normalize].max() - " << dataSet << "[cols_to_normalize].min() + 1e-9)\n";
            } else {
                pythonCode << dataSet << "[cols_to_normalize] = 2 * ((" << dataSet << "[cols_to_normalize] - " << dataSet << "[cols_to_normalize].min()) / (" << dataSet << "[cols_to_normalize].max() - " << dataSet << "[cols_to_normalize].min() + 1e-9)) - 1\n";
            }
        } else {
            pythonCode << "from sklearn.preprocessing import RobustScaler\n";
            pythonCode << "scaler = RobustScaler()\n";
            pythonCode << dataSet << "[cols_to_normalize] = scaler.fit_transform(" << dataSet << "[cols_to_normalize])\n";
        }
    } else {
        if (method == "min-max") {
            pythonCode << "numeric_cols = " << dataSet << ".select_dtypes(include=[np.number]).columns\n";
            if (rangeType == "0_1") {
                pythonCode << dataSet << "[numeric_cols] = (" << dataSet << "[numeric_cols] - " << dataSet << "[numeric_cols].min()) / (" << dataSet << "[numeric_cols].max() - " << dataSet << "[numeric_cols].min() + 1e-9)\n";
            } else {
                pythonCode << dataSet << "[numeric_cols] = 2 * ((" << dataSet << "[numeric_cols] - " << dataSet << "[numeric_cols].min()) / (" << dataSet << "[numeric_cols].max() - " << dataSet << "[numeric_cols].min() + 1e-9)) - 1\n";
            }
        } else {
            pythonCode << "from sklearn.preprocessing import RobustScaler\n";
            pythonCode << "scaler = RobustScaler()\n";
            pythonCode << "numeric_cols = " << dataSet << ".select_dtypes(include=[np.number]).columns\n";
            pythonCode << dataSet << "[numeric_cols] = scaler.fit_transform(" << dataSet << "[numeric_cols])\n";
        }
    }

    return visitChildren(ctx);
}

std::any PythonGenerator::visitStandardizeStat(MLScriptParser::StandardizeStatContext *ctx) {
    std:: string dataSet = ctx->IDENTIFIER()->getText();
    pythonCode << "# Standardize dataset (z-score normalization) "  << "\n";
    if (ctx->columnList()){
        std::string columns = getColumnList(ctx->columnList());
        pythonCode<< "cols= [" << columns << "]\n";
        pythonCode<<dataSet << "[cols] = (" << dataSet << "[cols] - " << dataSet << "[cols].mean()) / " << dataSet << "[cols].std()\n";

    }
    else{
        pythonCode << dataSet << " = (" << dataSet << " - " << dataSet << ".mean()) / " << dataSet << ".std()\n";    
    }

    return visitChildren(ctx);    
}



