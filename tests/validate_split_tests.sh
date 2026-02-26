#!/bin/bash
# Validation script for split RASA test files
# Compiles and runs all 5 test files

set -e

TEST_DIR="$(cd "$(dirname "$0")" && pwd)"
BGPQ4_DIR="$(dirname "$TEST_DIR")"

echo "========================================"
echo "RASA Split Test Validation"
echo "========================================"
echo ""

# Test files
TEST_FILES=(
    "test_rasa_auth.c"
    "test_rasa_set.c"
    "test_rasa_bidirectional.c"
    "test_rasa_bidirectional2.c"
    "test_rasa_edge.c"
)

# Expected test counts
EXPECTED_TESTS=(41 40 6 20 10)
TOTAL_EXPECTED=117

cd "$TEST_DIR"

PASS_COUNT=0
FAIL_COUNT=0

for i in "${!TEST_FILES[@]}"; do
    file="${TEST_FILES[$i]}"
    expected="${EXPECTED_TESTS[$i]}"
    base="${file%.c}"
    
    echo "----------------------------------------"
    echo "Testing: $file"
    echo "----------------------------------------"
    
    # Compile
    echo -n "  Compiling... "
    if cc -o "$base" "$file" -I"$BGPQ4_DIR" -L"$BGPQ4_DIR" -lrasa -ljansson 2>/dev/null; then
        echo "OK"
    else
        echo "FAIL"
        echo "  Trying alternative compile..."
        if cc -o "$base" "$file" -I"$BGPQ4_DIR" -L"$BGPQ4_DIR" -lrasa -ljansson -Wl,-rpath,"$BGPQ4_DIR" 2>&1 | head -5; then
            echo "OK (alternative)"
        else
            echo "FAIL - compilation error"
            FAIL_COUNT=$((FAIL_COUNT + 1))
            continue
        fi
    fi
    
    # Run tests
    echo -n "  Running tests... "
    if output=$("$TEST_DIR/$base" 2>&1); then
        passed=$(echo "$output" | grep -oE '[0-9]+/[0-9]+ tests passed' | head -1)
        echo "OK ($passed)"
        echo "$output" | tail -3
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "FAIL"
        echo "$output" | tail -5
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
    
    echo ""
done

echo "========================================"
echo "Validation Summary"
echo "========================================"
echo "Files compiled: $PASS_COUNT/$((PASS_COUNT + FAIL_COUNT))"
echo "Total tests: $TOTAL_EXPECTED (expected)"
echo ""

if [ $FAIL_COUNT -eq 0 ]; then
    echo "SUCCESS: All test files compiled and ran successfully!"
    exit 0
else
    echo "FAILURE: $FAIL_COUNT file(s) had issues"
    exit 1
fi
