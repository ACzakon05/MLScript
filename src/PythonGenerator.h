#pragma once
#include "MLScriptBaseVisitor.h"
#include <sstream>
#include <string>
#include <iostream>



class PythonGenerator : public MLScriptBaseVisitor {
public:
    std::stringstream pythonCode;

    std::any visitProg(MLScriptParser::ProgContext *ctx) override {
        pythonCode << "import pandas as pd\n";

        return visitChildren(ctx);
    }

    std::any visitLoadStat(MLScriptParser::LoadStatContext *ctx) override {
        std::string fileName = ctx->STRING()->getText();
        std::string varName = ctx->IDENTIFIER()->getText();

        pythonCode << varName << " = pd.read_csv(" << fileName << ")\n";

        if (ctx->KEEP()) {
            pythonCode << varName << " = " << varName << "[[" << getColumnList(ctx->columnList()) << "]]\n";
        } else if (ctx->WITHOUT()) {
            pythonCode << varName << " = " << varName << ".drop(columns=[" << getColumnList(ctx->columnList()) << "])\n";
        }

        return visitChildren(ctx);
    }

    std::any visitShowDataset(MLScriptParser::ShowDatasetContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();

        pythonCode << "print(" << varName << ")\n";

        return visitChildren(ctx);
    }

    std::any visitShowFeatures(MLScriptParser::ShowFeaturesContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();

        pythonCode << "print(f'Columns in " << varName << ": {" << varName << ".columns.tolist()}')\n"; 

        return visitChildren(ctx);
    }

    std::any visitShowCount(MLScriptParser::ShowCountContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();

        if (ctx->FEATURES()) {
            pythonCode << "print(f'Numer of features in " << varName << ": {len(" << varName << ".columns)}')\n";
        } 
        else if (ctx->ROWS()) {
            pythonCode << "print(f'Numer of rows in " << varName << ": {len(" << varName << ")}')\n";
        }

        return visitChildren(ctx);
    }

    std::any visitShowSingleRow(MLScriptParser::ShowSingleRowContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();
        std::string rowNumber = ctx->INTEGER()->getText();

        pythonCode << "print('" << varName << " row at position " << rowNumber << ":')\n";
        pythonCode << "print(f'{" << varName << ".iloc[" << rowNumber << "]}')\n";

        return visitChildren(ctx);
    }

    std::any visitShowMultipleRows(MLScriptParser::ShowMultipleRowsContext *ctx) override {
        std::string varName = ctx->IDENTIFIER()->getText();
        std::string lowerBound = ctx->INTEGER(0)->getText();
        std::string upperBound = std::to_string(std::stoi(ctx->INTEGER(1)->getText()) + 1);

        pythonCode << "print('" << varName << " rows from position " << lowerBound << " to position " << upperBound << ":')\n";
        pythonCode << "print(f'{" << varName << ".iloc[" << lowerBound << ":" << upperBound << "]}')\n";

        return visitChildren(ctx);
    }

    std::any visitSetTargetStat(MLScriptParser::SetTargetStatContext *ctx) override {
        std::string dataSet = ctx->IDENTIFIER()->getText();
        std::string targetCol = ctx->STRING()->getText();
        pythonCode<< "#Set target column for " << dataSet << ": " << targetCol << "\n";
        pythonCode << dataSet << "_target = " << dataSet << "[" << targetCol << "]\n";
        
        return visitChildren(ctx);
    }   
    //SPLIT my_dataset RATIO 80:20 INTO train_subset, test_subset WITH SEED 42, SHUFFLE true;
    std::any visitSplitStat(MLScriptParser:: SplitStatContext *ctx) override {
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


        
      
    std::any visitShowSingleFeature(MLScriptParser::ShowSingleFeatureContext *ctx) override {
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

private:
    std::string getColumnList(MLScriptParser::ColumnListContext *ctx) {
        std::string list;
        auto strings = ctx->STRING();

        for (size_t i = 0; i < strings.size(); ++i) {
            list += strings[i]->getText();
            if (i < strings.size() - 1) list += ", ";
        }
        
        return list;
    }
};