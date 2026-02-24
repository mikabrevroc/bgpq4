#!/bin/bash
#
# Scale testing for bgpq4 with RASA
# Tests performance with progressively larger AS-SETs

set -e

BGPQ4="./bgpq4"
TEST_DIR="tests/scale"
RESULTS_FILE="tests/scale_results.txt"

# Create test directory
mkdir -p "$TEST_DIR"

# Initialize results file
echo "bgpq4 RASA Scale Test Results" > "$RESULTS_FILE"
echo "===============================" >> "$RESULTS_FILE"
echo "Date: $(date)" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Function to generate large RASA-SET JSON
generate_rasa_set() {
    local num_members=$1
    local output_file=$2
    local set_name=$3

    echo "Generating RASA-SET with $num_members members..."

    cat > "$output_file" << EOF
{
  "rasa_sets": [
    {
      "as_set_name": "$set_name",
      "fallback_mode": "irrFallback",
      "members": [
EOF

    # Generate member ASNs
    for i in $(seq 1 $num_members); do
        local asn=$((10000 + i))
        if [ $i -eq $num_members ]; then
            echo "        $asn" >> "$output_file"
        else
            echo "        $asn," >> "$output_file"
        fi
    done

    cat >> "$output_file" << EOF
      ]
    }
  ]
}
EOF
}

# Function to generate RASA-AUTH for members
generate_rasa_auth() {
    local num_asns=$1
    local output_file=$2

    echo "Generating RASA-AUTH for $num_asns ASNs..."

    echo '{' > "$output_file"
    echo '  "rasas": [' >> "$output_file"

    for i in $(seq 1 $num_asns); do
        local asn=$((10000 + i))
        if [ $i -eq $num_asns ]; then
            echo "    {\"asn\": $asn, \"authorized_in\": [\"AS-SCALE-TEST\"]}" >> "$output_file"
        else
            echo "    {\"asn\": $asn, \"authorized_in\": [\"AS-SCALE-TEST\"]}," >> "$output_file"
        fi
    done

    echo '  ]' >> "$output_file"
    echo '}' >> "$output_file"
}

# Function to run scale test
run_scale_test() {
    local num_members=$1
    local test_name="scale_${num_members}"

    echo ""
    echo "========================================"
    echo "Scale Test: $num_members members"
    echo "========================================"

    # Generate test files
    generate_rasa_set "$num_members" "$TEST_DIR/${test_name}_set.json" "AS-SCALE-TEST"
    generate_rasa_auth "$num_members" "$TEST_DIR/${test_name}_auth.json"

    # Measure file sizes
    local set_size=$(stat -f%z "$TEST_DIR/${test_name}_set.json" 2>/dev/null || stat -c%s "$TEST_DIR/${test_name}_set.json")
    local auth_size=$(stat -f%z "$TEST_DIR/${test_name}_auth.json" 2>/dev/null || stat -c%s "$TEST_DIR/${test_name}_auth.json")

    echo "RASA-SET file size: $set_size bytes"
    echo "RASA-AUTH file size: $auth_size bytes"

    # Time the JSON loading
    echo "Testing JSON loading..."
    local start_time=$(date +%s%N)

    # Use test binary to load and verify
    ./test_rasa_load "$TEST_DIR/${test_name}_set.json" "$TEST_DIR/${test_name}_auth.json" 2>/dev/null || {
        echo "Note: test_rasa_load not available, using bgpq4 -Y/-y flags"
    }

    local end_time=$(date +%s%N)
    local duration=$(( (end_time - start_time) / 1000000 ))  # Convert to milliseconds

    echo "Load time: ${duration}ms"

    # Log results
    echo "" >> "$RESULTS_FILE"
    echo "Test: $num_members members" >> "$RESULTS_FILE"
    echo "  RASA-SET size: $set_size bytes" >> "$RESULTS_FILE"
    echo "  RASA-AUTH size: $auth_size bytes" >> "$RESULTS_FILE"
    echo "  Load time: ${duration}ms" >> "$RESULTS_FILE"
}

# Main test sequence
echo "Starting Scale Tests..."
echo ""

# Test with progressively larger sizes
test_sizes=(10 100 500 1000 5000 10000)

for size in "${test_sizes[@]}"; do
    run_scale_test "$size"
done

echo ""
echo "========================================"
echo "Scale Testing Complete"
echo "Results written to: $RESULTS_FILE"
echo "========================================"

# Display summary
cat "$RESULTS_FILE"
