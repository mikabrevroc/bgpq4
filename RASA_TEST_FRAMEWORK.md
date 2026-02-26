# Comprehensive Test Framework Design for bgpq4

## Executive Summary

This document outlines a comprehensive test creation framework for bgpq4 that systematically tests all CLI flag permutations, edge cases, and integrations. The framework will generate 1000+ test cases programmatically while avoiding nonsensical flag combinations.

**Key Goals:**
- Generate tests programmatically using Python
- Cover all valid flag combinations (avoid mutually exclusive flags)
- Test all 16 vendor formats, 7 generation types, and 20+ modifiers
- Support mocking of IRR queries (no external dependencies)
- Integrate with existing C test infrastructure
- Enable regression testing and CI/CD integration

---

## 1. Architecture Overview

### System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Test Generation Layer                    │
│  (Python framework: schema, combinations, test generators)  │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                  Template & Code Generation                 │
│   (C test templates, mock data, expected outputs)          │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                   Test Execution Layer                      │
│  (Unit tests, integration tests, output comparison)        │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                 Validation & Reporting                      │
│          (Coverage reports, regression detection)           │
└─────────────────────────────────────────────────────────────┘
```

### Key Components

1. **Python Test Generator** (`tests/generate/`)
   - `flag_schema.py` - Defines all CLI flags, types, constraints
   - `combination_engine.py` - Generates valid flag combinations
   - `test_generator.py` - Creates C test files from templates
   - `mock_irr.py` - Provides mock IRR query responses

2. **Template System** (`tests/templates/`)
   - `unit_test_template.c` - Template for unit tests
   - `integration_test_template.c` - Template for integration tests
   - `output_validation_template.c` - Template for output comparison

3. **Mock Infrastructure** (`tests/mocks/`)
   - `irr_mock.c` - Mock IRR server responses
   - `rasa_mock.c` - Mock RASA data
   - `fixture_loader.c` - Load test fixtures

4. **Configuration** (`tests/config/`)
   - `test_matrix.yaml` - Defines which combinations to test
   - `skip_conditions.yaml` - Defines invalid flag combinations
   - `test_priorities.yaml` - Priority levels for different tests

---

## 2. Flag Categorization and Valid Combinations

### 2.1 Flag Categories

#### Category A: Vendor Targets (Mutually Exclusive)
- `-X` (Cisco XR)
- `-U` (Huawei)
- `-u` (Huawei XPL)
- `-j` (JSON)
- `-J` (Juniper)
- `-K` (MikroTik v6)
- `-K7` (MikroTik v7)
- `-b` (BIRD)
- `-B` (OpenBGPD)
- `-N` (Nokia Classic)
- `-n` (Nokia MD)
- `-n2` (Nokia SRL)
- `-e` (Arista)
- `-F fmt` (Custom format)
- **Default**: Cisco IOS (when no vendor flag specified)

#### Category B: Generation Types (Mutually Exclusive)
- `-f num` (Input AS-path)
- `-G num` (Output AS-path)
- `-H num` (Origin AS-list, Juniper only)
- `-t` (AS-sets, OpenBGPD/BIRD/JSON only)
- `-E` (Extended ACL/Route-filter)
- `-z` (Route-filter-list, JunOS only)
- **Default**: Prefix-list (when no type flag)

#### Category C: Input Filters (Compatible)
- `-4` (IPv4, default)
- `-6` (IPv6)
- `-m len` (Max prefix length)
- `-L depth` (Recursion limit)
- `-S sources` (IRR sources)
- `-w` (Validate ASNs)

#### Category D: Output Modifiers (Compatible)
- `-3` (32-bit ASN, default)
- `-A` (Aggregate prefixes)
- `-l name` (List name)
- `-p` (Allow special ASNs)
- `-R len` (More specific up to)
- `-r len` (More specific from)
- `-s` (Sequence numbers, IOS only)
- `-M match` (Extra match, Juniper only)
- `-W len` (Max AS-path entries)

#### Category E: RASA Options (Compatible)
- `-Y file` (RASA-SET JSON file)
- `-y file` (RASA-AUTH JSON file)

#### Category F: Utility Options (Independent)
- `-d` (Debug)
- `-h host` (IRR server)
- `-T` (No pipelining)
- `-v` (Version)

### 2.2 Combination Rules

#### Mutually Exclusive Groups

**Group 1: Vendors** (select 0 or 1)
- Cannot combine multiple vendor flags
- `-F` conflicts with vendors (custom format is a vendor)
- Default is Cisco IOS when none specified

**Group 2: Generation Types** (select 0 or 1)
- Only one generation type per invocation
- `-E`, `-f`, `-G`, `-H`, `-t`, `-z` are mutually exclusive
- Default is prefix-list generation

**Group 3: IP Version** (select 0 or 1)
- `-4` and `-6` are mutually exclusive
- `-4` is the default

#### Conditional Exclusivity

- `-H` (origin AS-list) only works with Juniper (`-J`)
- `-z` (route-filter-list) only works with Juniper (`-J`)
- `-s` (sequence numbers) only works with Cisco IOS (default or `-X`)
- `-t` (AS-sets) requires OpenBGPD (`-B`), BIRD (`-b`), or JSON (`-j`)

#### Valid Combinations

The framework will generate tests for all valid combinations following these rules:

1. **Pick one vendor** (or default to Cisco IOS)
2. **Pick one generation type** (or default to prefix-list)
3. **Pick IP version** (-4 or -6, defaults to -4)
4. **Apply compatible output modifiers** (respecting vendor/type restrictions)
5. **Apply input filters** (any combination is valid)
6. **Include RASA tests** when applicable (requires JSON support)

### 2.3 Estimated Test Count

Based on the permutation analysis:

- **Vendors**: 15 options (including default)
- **Generation Types**: 6 options (including default prefix-list)
- **IP Versions**: 2 options (-4, -6)
- **Modifiers**: Each modifier can be present/absent (2^n combinations)

**Conservative Estimate**: 1000-2000 valid test cases

---

## 3. Test Case Generation Strategy

### 3.1 Test Categories

#### Unit Tests (500-800 tests)
- Test individual flag functionality
- Test combination interactions
- Test edge cases (empty sets, recursion limits, ASN 32-bit overflow)
- Test validation logic

#### Integration Tests (200-300 tests)
- End-to-end with mocked IRR
- RASA integration with validation
- Multi-AS-SET expansion
- Error handling and recovery

#### Output Validation Tests (300-400 tests)
- Compare generated output to reference files
- Test all vendor formats
- Test all generation types
- Verify syntactic correctness per vendor

#### Performance Tests (50-100 tests)
- Large AS-SET expansion (1000+ members)
- Deep recursion (10+ levels)
- Memory usage validation
- Timeout handling

### 3.2 Test Generation Strategy

```python
# Pseudo-code for test generation

def generate_test_cases():
    tests = []
    
    for vendor in VENDORS:
        for gen_type in GENERATION_TYPES:
            if not is_valid_combination(vendor, gen_type):
                continue
                
            for af in [4, 6]:
                if not is_valid_af_combination(vendor, gen_type, af):
                    continue
                    
                # Test minimal case
                tests.append(generate_minimal_test(vendor, gen_type, af))
                
                # Test with various modifiers
                for modifier_set in generate_modifier_combinations(vendor, gen_type):
                    tests.append(generate_modified_test(vendor, gen_type, af, modifier_set))
                
                # Test with input filters
                for filter_set in generate_filter_combinations():
                    tests.append(generate_filtered_test(vendor, gen_type, af, filter_set))
    
    return tests
```

### 3.3 Mock Data Strategy

#### Fixture-Based Testing

```
tests/fixtures/
├── irr_responses/
│   ├── as112.txt          # Small AS with routes
│   ├── as_hurricane.txt   # Large AS-SET test
│   ├── depth_test.txt     # Deep recursion (10+ levels)
│   ├── special_asn.txt    # Private/32-bit ASNs
│   └── error_response.txt # IRR errors
├── rasa_data/
│   ├── rasa_set_valid.json
│   ├── rasa_auth_valid.json
│   ├── rasa_partial.json  # Some authorized, some not
│   └── rasa_empty.json
└── expected_outputs/
    ├── ios--simple.txt
    ├── junos--aggregated.txt
    └── bird--aspath.txt
```

### 3.4 Test Coverage Matrix

| Feature | Unit | Integration | Output | Performance |
|---------|------|-------------|--------|-------------|
| Vendors | ✓ | ✓ | ✓ | - |
| Generation Types | ✓ | ✓ | ✓ | - |
| Input Filters | ✓ | ✓ | ✓ | - |
| Output Modifiers | ✓ | ✓ | ✓ | - |
| RASA Integration | ✓ | ✓ | ✓ | - |
| Error Handling | ✓ | ✓ | - | - |
| Edge Cases | ✓ | ✓ | ✓ | - |
| Large Data Sets | - | ✓ | ✓ | ✓ |
| Deep Recursion | ✓ | ✓ | ✓ | ✓ |

---

## 4. File Structure

```
bgpq4/
├── tests/                          # Main test directory
│   ├── generate/                   # Python test generators
│   │   ├── __main__.py            # Entry point
│   │   ├── flag_schema.py         # Flag definitions
│   │   ├── combination_engine.py  # Combo generator
│   │   ├── test_generator.py      # C test generator
│   │   └── mock_irr.py            # IRR mock server
│   ├── templates/                  # C test templates
│   │   ├── header.h.template      # Common includes
│   │   ├── unit_test.c.template   # Unit test template
│   │   ├── integration.c.template # Integration test template
│   │   └── runner.c.template      # Test runner
│   ├── mocks/                      # Mock implementations
│   │   ├── irr_mock.c             # Mock IRR queries
│   │   ├── rasa_mock.c            # Mock RASA data
│   │   └── fixture_loader.c       # Test fixture loader
│   ├── fixtures/                   # Test data
│   │   ├── irr_responses/         # Mock IRR data
│   │   ├── rasa_data/             # RASA test configs
│   │   └── inputs/                # Input AS-SETs
│   └── config/                     # Test configuration
│       ├── test_matrix.yaml       # Test definitions
│       ├── skip_conditions.yaml   # Invalid combos
│       └── priorities.yaml        # Test priorities
├── tests_generated/               # Generated test files
│   ├── unit/
│   ├── integration/
│   └── output/
└── test_runner.py                 # Main test runner
```

---

## 5. Implementation Approach

### 5.1 Phase 1: Foundation (1-2 weeks)

**Deliverables:**
- Python test generator framework structure
- Flag schema definition (all CLI options)
- Combination engine with validation logic
- Mock IRR server implementation

**Tasks:**
1. Create directory structure
2. Implement `flag_schema.py` with all CLI flags
3. Implement `combination_engine.py` with validation rules
4. Create mock IRR server that serves fixture data
5. Write unit tests for the generator framework

### 5.2 Phase 2: Template System (1 week)

**Deliverables:**
- C unit test templates
- Integration test templates
- Expected output validation templates

**Tasks:**
1. Design test template system
2. Create unit test template with assertions
3. Create integration test template with setup/teardown
4. Implement output comparison logic
5. Integrate with existing automake test infrastructure

### 5.3 Phase 3: Test Generation (2-3 weeks)

**Deliverables:**
- Generated test files for all valid combinations
- Fixture data for common scenarios
- Reference output files

**Tasks:**
1. Run generation for basic unit tests (simple flag combinations)
2. Create fixture data for AS112, AS-HURRICANE, etc.
3. Generate integration tests with mock IRR
4. Create reference outputs for comparison tests
5. Implement continuous test generation in CI

### 5.4 Phase 4: Enhancement & Optimization (1-2 weeks)

**Deliverables:**
- Performance test suite
- Edge case coverage
- RASA integration tests
- Documentation and examples

**Tasks:**
1. Generate performance tests for large AS-SETs
2. Add edge case tests (empty sets, loops, max values)
3. Create RASA comprehensive integration tests
4. Optimize test execution time
5. Write framework documentation

### 5.5 Phase 5: Integration & CI/CD (1 week)

**Deliverables:**
- CI/CD pipeline integration
- Test reporting
- Coverage analysis
- Developer documentation

**Tasks:**
1. Integrate with GitHub Actions workflow
2. Set up test result reporting
3. Add code coverage with gcov/lcov
4. Create developer README
5. Run full test suite on PRs

---

## 6. Example Generated Test Cases

### Example 1: Basic Unit Test

**Generated from:** `bgpq4 -4 -b AS112`

```c
// tests_generated/unit/test_prefixlist_bird_ipv4.c
#include "test_framework.h"
#include "mock_irr.h"
#include "bgpq4_test_common.h"

TEST_CASE(test_bird_ipv4_prefixlist_as112)
{
    struct bgpq_expander *expander;
    FILE *output;
    char *result;
    
    // Setup
    mock_irr_load_fixture("as112.txt");
    expander = bgpq_expander_test_create();
    expander->vendor = V_BIRD;
    expander->family = AF_INET;
    expander->generation = T_PREFIXLIST;
    
    // Execute
    bgpq_expander_add_as(expander, "AS112");
    bgpq_expand(expander);
    
    // Verify
    output = tmpfile();
    bgpq4_print_prefixlist(output, expander);
    result = read_file_contents(output);
    
    ASSERT_STR_CONTAINS(result, "AS112");
    ASSERT_STR_CONTAINS(result, "192.0.2.0/24");
    
    // Cleanup
    bgpq_expander_destroy(expander);
    fclose(output);
    free(result);
}
```

### Example 2: Integration Test with Output Validation

**Generated from:** `bgpq4 -6 -J -E -A -l test1 AS112`

```c
// tests_generated/integration/test_junos_eacl_ipv6.c
#include "test_framework.h"
#include "mock_irr.h"
#include "output_validator.h"

TEST_CASE(test_junos_eacl_ipv6_aggregated_as112)
{
    struct test_fixture *fixture;
    struct bgpq_expander *expander;
    FILE *output, *expected;
    char *cmdline = "bgpq4 -6 -J -E -A -l test1 AS112";
    
    // Setup
    fixture = load_fixture("as112_ipv6.txt");
    expected = open_expected_output("junos-eacl-aggregated.txt");
    expander = create_expander_from_argv(cmdline);
    
    // Execute with mock IRR
    mock_irr_respond_with(fixture->responses);
    ASSERT_EQ_INT(0, cli_main(expander, "AS112"));
    
    // Verify output matches reference
    output = get_captured_output();
    ASSERT_FILES_MATCH(output, expected);
    
    // Cleanup
    destroy_fixture(fixture);
}
```

### Example 3: RASA Integration Test

**Generated from:** `bgpq4 -4 -b -Y rasa_set.json -y rasa_auth.json AS-HURRICANE`

```c
// tests_generated/integration/test_rasa_bird_hurricane.c
#include "test_framework.h"
#include "mock_irr.h"
#include "mock_rasa.h"

TEST_CASE(test_rasa_bird_hurricane_expansion)
{
    struct rasa_fixture *rasa_fix;
    struct bgpq_expander *expander;
    int result_count = 0;
    
    // Setup RASA data
    rasa_fix = mock_rasa_load("hurricane_test.json");
    expander = bgpq_expander_test_create();
    expander->vendor = V_BIRD;
    ASSERT_EQ_INT(0, bgpq_rasa_load(expander->rasa_set, RASA_SET_JSON));
    ASSERT_EQ_INT(0, bgpq_rasa_auth_load(expander->rasa, RASA_AUTH_JSON));
    
    // Load IRR fixtures
    mock_irr_load_as_set("AS-HURRICANE", 500); // 500 member ASNs
    
    // Execute expansion with RASA validation
    bgpq_expander_add_asset(expander, "AS-HURRICANE");
    result_count = bgpq_expand_with_rasa(expander);
    
    // Verify: Should filter out unauthorized ASNs
    ASSERT_LT_INT(result_count, 500);
    ASSERT_GT_INT(result_count, 50);  // Just an approximate range
    
    // Verify no unauthorized ASNs in output
    ASSERT_NO_UNAUTHORIZED(expander, rasa_fix->authorized_asns);
    
    // Cleanup
    mock_rasa_destroy(rasa_fix);
    bgpq_expander_destroy(expander);
}
```

### Example 4: Edge Case Test

```c
// tests_generated/unit/test_edge_cases.c

TEST_CASE(test_empty_as_set_with_rasa)
{
    // Empty AS-SET should not crash
    expander = setup_expander();
    mock_irr_load_fixture("empty.txt");
    
    int result = bgpq_expand(expander);
    ASSERT_EQ_INT(-1, result);  // Should return error
}

TEST_CASE(test_recursion_limit_exceeded)
{
    // Deep recursion should respect -L flag
    expander = setup_expander();
    expander->maxdepth = 3;
    mock_irr_load_fixture("deep_recursion.txt");  // 10 levels deep
    
    int result = bgpq_expand(expander);
    ASSERT_EQ_INT(0, result);  // Should succeed but stop at depth 3
    ASSERT_LE_INT(count_asns(expander), 100);  // Depth 3 shouldn't exceed 100
}

TEST_CASE(test_special_asn_handling)
{
    // 23456 (AS_TRANS) and private ASNs
    expander = setup_expander();
    expander->expand_special_asn = 1;  // -p flag
    
    mock_irr_load_fixture("special_asns.txt");
    bgpq_expand(expander);
    
    ASSERT_ASN_PRESENT(expander, 23456);
    ASSERT_ASN_PRESENT(expander, 64512);  // Private ASN
}
```

---

## 7. Integration with Existing Test Infrastructure

### 7.1 Automake Integration

Integrate generated tests into existing `Makefile.am`:

```makefile
# tests/Makefile.am.inc

# Generated test programs
GENERATED_CHECK_PROGRAMS = \
    tests_generated/test_prefixlist_ios \
    tests_generated/test_prefixlist_junos \
    tests_generated/test_aggregation \
    tests_generated/test_rasa_integration \
    ... 100+ more test programs

# Add to existing check_PROGRAMS
check_PROGRAMS += $(GENERATED_CHECK_PROGRAMS)

# Auto-generate test sources
tests_generated/%.c: tests/generator_force
	python3 tests/generate_test_suites.py --output-dir=tests_generated
```

### 7.2 Test Execution Workflow

```bash
# Full test suite execution
make check  # Runs both existing and generated tests

# Parallel execution (recommended for CI)
make -j$(nproc) check

# Run specific test categories
./tests/run_test_category.py unit
./tests/run_test_category.py integration
./tests/run_test_category.py performance

# Generate and verify reference outputs
./tests/generate_outputs.sh ./bgpq4 tests/reference
./tests/compare_outputs.sh tests/reference tests_generated/outputs
```

### 7.3 CI/CD Integration

**GitHub Actions integration:**

```yaml
# .github/workflows/generated-tests.yml
name: Comprehensive Test Suite

on: [push, pull_request]

jobs:
  generate-and-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      - name: Generate tests
        run: |
          python3 -m pip install pyyaml jinja2
          python3 tests/generate/__main__.py
      
      - name: Configure and build
        run: |
          autoreconf -fi
          ./configure
          make -j$(nproc)
      
      - name: Run unit tests
        run: make check
      
      - name: Run integration tests
        run: ./tests/run_integration_suite.sh
      
      - name: Generate and compare outputs
        run: |
          ./tests/generate_outputs.sh ./bgpq4 tests/outputs
          diff -qr tests/reference tests/outputs
      
      - name: Upload coverage
        uses: codecov/codecov-action@v2
        with:
          files: ./coverage.info
```

---

## 8. Mock System Design

### 8.1 Mock IRR Server

```c
// tests/mocks/irr_mock.h

struct mock_irr_response {
    char *query;
    char *response;  // Multi-line response
    enum irr_query_type type;
};

void mock_irr_init(void);
void mock_irr_shutdown(void);
void mock_irr_load_fixture(const char *filename);
void mock_irl_expect_query(const char *query, const char *response);

// Intercept actual IRR queries
ssize_t mock_irr_write(int fd, const void *buf, size_t count);
ssize_t mock_irr_read(int fd, void *buf, size_t count);
```

### 8.2 Fixture System

```yaml
# tests/fixtures/as-hurricane.yaml
fixture:
  name: "AS-HURRICANE"
  description: "Large AS-SET for performance testing"
  
  # Simulated IRR responses
  irr_responses:
    - query: "-i origin AS11042"
      response: |
        route: 192.158.180.0/24
        origin: AS11042
        route: 207.45.222.0/24
        origin: AS11042
    
    - query: "-i origin AS13768"
      response: |
        route: 23.147.112.0/24
        origin: AS13768
        route: 23.147.113.0/24
        origin: AS13768
  
  # Expected results after expansion
  expected_prefixes: 5000
  expected_unique_asns: 350
```

### 8.3 RASA Mock Data

```json
{
  "fixture": "hurricane_rasa_test",
  "rasa_set": {
    "AS-HURRICANE": {
      "comment": "NTT Global IP Network",
      "members": ["AS11042", "AS13768", "AS18121", ...],
      "source": "RIPE"
    }
  },
  "rasa_auth": {
    "authorization_entries": [
      {
        "as_number": "AS11042",
        "authorized_set_names": ["AS-HURRICANE"],
        "timestamp": "2024-01-01T00:00:00Z"
      }
    ]
  }
}
```

---

## 9. Edge Cases and Error Conditions

### 9.1 Error Scenarios to Test

#### IRR Communication Errors
- Network timeout (simulate slow IRR)
- IRR server unavailable
- Malformed responses
- Empty responses
- Truncated data

#### Invalid Input
- Non-existent AS-SET names
- Invalid ASN format
- Incomplete filter specifications
- Missing required data

#### Edge Cases
1. **Empty Results**: AS-SET with no members
2. **Single Element**: AS-SET with exactly one member
3. **Self-Reference**: AS-SET includes itself (circular dependency)
4. **Maximum Values**:
   - Maximum prefix length (/32 IPv4, /128 IPv6)
   - Maximum AS-PATH length (-W flag)
   - Maximum recursion depth (-L flag)
5. **Special ASNs**:
   - AS_TRANS (23456)
   - AS_RSVD (0, 65535, 4294967295)
   - Private ASNs (64512-65534, 4200000000-4294967294)
   - 32-bit ASNs > 65535

#### Performance Edge Cases
- AS-SET with 10,000+ members
- Recursion depth > 20
- Multiple identical prefixes
- Massive route-filters

### 9.2 Expected Behavior Matrix

| Scenario | Expected Behavior | Error Code |
|----------|------------------|------------|
| Empty AS-SET | Return 0 prefixes | Exit 0 (success) |
| Non-existent AS-SET | Print error to stderr | Exit 1 |
| Recursion limit hit | Gracefully truncate | Exit 0 (partial success) |
| IRR timeout | Print timeout error | Exit 2 |
| Invalid ASN | Parse error | Exit 1 |
| Memory allocation failure | Print "out of memory" | Exit 255 |
| Invalid flag combination | Print usage help | Exit 1 |

---

## 10. Test Prioritization

### P0: Critical (Must Pass)
- Core functionality (basic prefix generation)
- All vendor formats generate valid syntax
- RASA authorization filtering works
- Error conditions handled gracefully

### P1: Important (Should Pass)
- All flag combinations work as documented
- Output modifiers function correctly
- Aggregation produces correct results
- Input filters work as expected

### P2: Nice to Have (Best Effort)
- Performance meets targets
- Edge cases handled gracefully
- Memory usage reasonable
- Documentation examples work

---

## 11. Success Criteria and Metrics

### 11.1 Milestones

1. **MVP**: 100 unit tests for basic flag combinations (Week 2)
2. **Alpha**: 500 tests covering all vendors and types (Week 4)
3. **Beta**: 1000+ tests including integration (Week 6)
4. **GA**: Full suite with CI/CD, 90%+ coverage (Week 8)

### 11.2 Quality Metrics

- **Test Count**: 1000+ generated test cases
- **Code Coverage**: 90%+ for core bgpq4.c and expander.c
- **Coverage Time**: < 30 minutes for full test suite (parallel execution)
- **Bug Detection**: Zero regressions in manual testing
- **False Positive Rate**: < 1% (flaky test rate)

---

## 12. Maintenance and Extensibility

### 12.1 Adding New Tests

**To add a new test category:**
1. Define it in `tests/config/test_matrix.yaml`
2. Add fixture data if needed
3. Generate tests: `make generate-tests`
4. Run and verify: `make check`

**To add new CLI flags:**
1. Add to `flag_schema.py`
2. Define constraints in `combination_engine.py`
3. Update templates if needed
4. Regenerate tests

### 12.2 Test Updates

**When bgpq4 behavior changes:**
- Expected behavior: Update fixture in `config/test_matrix.yaml`
- **OR** flag to generate new reference outputs: `make update-references`

**When new features added:**
1. Add feature to schema
2. Generate tests for new feature
3. Add reference outputs
4. Include in regression suite

### 12.3 Continuous Maintenance

- **Weekly**: Review flaky tests, update as needed
- **On Release**: Generate full test suite, validate
- **On Bug**: Add regression test
- **On Feature**: Add comprehensive tests for feature

---

## 13. Risks and Mitigation

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Test generation takes too long | Medium | Low | Incremental generation, caching |
| Too many tests (>10,000) | High | Medium | Smart pruning, skip low-value combos |
| Flaky tests due to timing | Medium | Medium | Mock all time-dependent operations |
| Coverage lower than expected | High | Low | Add targeted unit tests for uncovered code |
| CI timeout | Medium | Low | Parallel execution, test prioritization |
| Maintenance burden | Medium | Medium | Automated updates, clear documentation |

---

## 14. Timeline and Resources

### 14.1 Estimated Effort

| Phase | Duration | Effort (hours) |
|-------|----------|----------------|
| Phase 1: Foundation | 1-2 weeks | 40-80 |
| Phase 2: Templates | 1 week | 40 |
| Phase 3: Generation | 2-3 weeks | 80-120 |
| Phase 4: Enhancement | 1-2 weeks | 40-80 |
| Phase 5: Integration | 1 week | 40 |
| **Buffer** | 1 week | 40 |
| **Total** | **7-10 weeks** | **280-400** |

### 14.2 Team Requirements

- **Lead Developer**: Python + C expertise (0.5 FTE)
- **Domain Expert**: bgpq4 internals (0.25 FTE)
- **CI/CD Engineer**: Integration (0.1 FTE)

---

## 15. Conclusion

This comprehensive test framework will:

1. **Ensure Quality**: Test all flag combinations systematically
2. **Prevent Regressions**: Catch issues before release
3. **Enable Refactoring**: Tests provide confidence for changes
4. **Document Behavior**: Tests serve as executable specifications
5. **Scale**: Framework adapts to new features automatically

The framework will position bgpq4 as a well-tested, enterprise-grade tool with clear quality guarantees and reliable behavior across all supported platforms and configurations.


## 16. RASA-Specific Test Scenarios (Critical)

### 16.1 Multi-Level AS-SET Authorization Tests

**Test Chain: 3-Level Hierarchy with Mixed Authorization**

```
AS-TIER1 (root)
├── AS64496 (authorized via RASA-AUTH for TIER1)
├── AS64497 (NOT authorized for TIER1)
└── AS-TIER2 (nested AS-SET)
    ├── AS64498 (authorized via RASA-AUTH for TIER2)
    ├── AS64499 (authorized for TIER1 but NOT TIER2)
    └── AS-TIER3
        ├── AS64500 (authorized for TIER3 only)
        └── AS64501 (no RASA-AUTH)
```

**Expected Results:**
- AS64496: INCLUDED (authorized for TIER1)
- AS64497: EXCLUDED (no authorization)
- AS64498: INCLUDED (authorized for TIER2, parent allows)
- AS64499: EXCLUDED at TIER2 (not authorized for TIER2)
- AS64500: INCLUDED at TIER3 (authorized for TIER3)
- AS64501: DEFAULT_ALLOW (no RASA-AUTH)

**Test Count**: 12 scenarios x 3 fallback modes = 36 tests

### 16.2 Fallback Mode Integration Tests

**irrLock Mode Tests:**
1. AS-SET with irrLock + valid irr_source → Query only specified IRR
2. AS-SET with irrLock + missing irr_source → ERROR
3. AS-SET with irrLock + empty members → Valid, locks to IRR only
4. irrLock with different sources → 12 tests

**rasaOnly Mode Tests:**
1. AS-SET with rasaOnly + members → Use only RASA members
2. AS-SET with rasaOnly + empty members → Authoritative empty
3. AS-SET with rasaOnly + nestedSets → Expand nested RASA-SETs
4. rasaOnly with doNotInherit → Stop at this level

**irrFallback Mode Tests:**
1. Merge RASA members with IRR results
2. Handle duplicates
3. RASA-SET precedence over IRR

**Test Count**: 20 scenarios x 3 modes = 60 tests

### 16.3 doNotInherit Flag Tests

1. Level 0 doNotInherit → Only TIER1 expanded
2. Level 1 doNotInherit → TIER1 and TIER2 expanded
3. Level 2 doNotInherit → All 3 levels expanded
4. Mixed with fallback modes → 12 tests

**Test Count**: 4 levels x 3 modes x 2 states = 24 tests

### 16.4 RASA Test Summary

**Total RASA-Specific Tests:**
- Multi-level authorization: 36
- Fallback modes: 60
- doNotInherit: 24
- Propagation scopes: 24
- Bidirectional verification: 10
- Circular references: 12
- Real-world scenarios: 30
- **Subtotal: 196 tests**

**With CLI flag permutations**: **2,000-5,000 high-value tests**

---

---

## Appendix A: Glossary

- **IRR**: Internet Routing Registry
- **RASA**: Routing Authorization and Security Architecture
- **AS-SET**: Group of autonomous systems in IRR
- **AS-PATH**: List of AS numbers in BGP path
- **NOS**: Network Operating System (vendor)
- **Fixture**: Predefined test data
- **Mock**: Simulated component for testing
- **Stub**: Simplified test double

---

## Appendix B: References

- Current test structure: `/tests/` directory
- CLI options: `main.c` lines 53-121 (usage function)
- Generation types: `extern.h` lines 74-82
- Vendor types: `extern.h` lines 55-71
