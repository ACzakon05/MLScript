#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
RESET='\033[0m'

./build.sh > /dev/null

echo "=== Running tests ==="
passed=0
failed=0

for test_file in tests/source/**/*.mls; do
    [[ -f "$test_file" ]] || continue

    rel_path="${test_file#tests/source/}"
    basename="${rel_path%.mls}"
    expected_file="tests/expected/${basename}.py"

    ./build/mlscript "$test_file" > /dev/null 2>&1

    if diff -u -w -B "mlscript.out.py" "$expected_file" > /dev/null; then
        echo -e " ${GREEN}[PASS] $basename${RESET}"
        ((passed++))
    else
        echo -e " ${RED}[FAIL] $basename${RESET}"
        echo -e " --- diff ---${BLUE}"
        diff -u -w -B "$expected_file" "mlscript.out.py" | sed 's/^/    /'
        echo -e " ${RESET}------------"
        ((failed++))
    fi
done

echo "=== Summary: $passed Passed, $failed Failed ==="
[ $failed -eq 0 ] && exit 0 || exit 1