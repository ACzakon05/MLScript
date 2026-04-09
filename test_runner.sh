#!/bin/bash

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
        echo " [PASS] $basename"
        ((passed++))
    else
        echo " [FAIL] $basename"
        ((failed++))
    fi
done

echo "=== Summary: $passed Passed, $failed Failed ==="
[ $failed -eq 0 ] && exit 0 || exit 1