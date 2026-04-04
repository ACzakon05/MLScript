#include <iostream>
#include <fstream>
#include <cstdlib>

// ANTLR Runtime Headers
#include "antlr4-runtime.h"

// Your Generated Headers
#include "MLScriptLexer.h"
#include "MLScriptParser.h"

// Transpiler
#include "PythonGenerator.h"

// Error handling
#include "CustomErrorListener.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <script_name.mls>\n";
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile.is_open()) {
        std::cerr << "[Compiler] Error: Could not open file " << argv[1] << "\n";
        return 1; 
    }

    antlr4::ANTLRInputStream input(inFile);
    inFile.close();

    CustomErrorListener errorListener;
    MLScriptLexer lexer(&input);

    lexer.removeErrorListeners();
    lexer.addErrorListener(&errorListener);

    antlr4::CommonTokenStream tokens(&lexer);
    MLScriptParser parser(&tokens);

    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);

    antlr4::tree::ParseTree *tree = parser.prog();

    if (errorListener.hasError) {
        std::cerr << "[Compiler] Compilation aborted due to errors\n";
        return 1;
    }

    PythonGenerator visitor;
    visitor.visit(tree);

    std::string pythonCode = visitor.pythonCode.str();

    std::cout << "[Compiler] Translation successful. Writing to mlscript.out.py\n";

    std::ofstream outFile("mlscript.out.py");
    outFile << pythonCode;
    outFile.close();

    return 0;
}