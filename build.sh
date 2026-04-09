#!/bin/bash
set -e


# Ścieżka do ANTLR JAR
ANTLR_JAR="$HOME/libs/antlr-4.13.2-complete.jar"
# Generating ANTLR files
echo "[Step 1/3] Generating ANTLR C++ files..."
java -Xmx500M -cp "$ANTLR_JAR:$CLASSPATH" org.antlr.v4.Tool -Dlanguage=Cpp -visitor -no-listener -o generated MLScript.g4

# Prepare build directory
echo "[Step 2/3] Configuring CMake..."
mkdir -p build
cd build

# Compile
echo "[Step 3/3] Compiling NeuroScript..."
cmake ..
make -j$(nproc)

echo "Done. You can now run: ./build/mlscript"