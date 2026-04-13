#include "PythonGenerator.h"

std::any PythonGenerator::visitSetTargetStat(MLScriptParser::SetTargetStatContext *ctx) {
    std::string dataSet = ctx->IDENTIFIER()->getText();
    std::string targetCol = ctx->COL_NAME()->getText();
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