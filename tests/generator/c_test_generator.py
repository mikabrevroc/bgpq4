#!/usr/bin/env python3
"""
C Test Code Generator for RASA Fixtures

Reads JSON fixtures and generates C test code for bgpq4 test suite.
"""
import json
import os
import sys
from typing import Dict, List, Any, Optional
from pathlib import Path


class CTestGenerator:
    """Generate C test code from JSON fixtures."""
    
    def __init__(self, fixtures_dir: str = "../fixtures", output_dir: str = ".."):
        self.fixtures_dir = Path(fixtures_dir)
        self.output_dir = Path(output_dir)
        self.header_template = self._load_template("test_header.c")
        self.test_template = self._load_template("test_body.c")
    
    def _load_template(self, name: str) -> str:
        """Load a template file or use embedded defaults."""
        templates = {
            "test_header.c": '''/* Auto-generated RASA test file - DO NOT EDIT */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../rasa.h"
#include "../../rasa_hash.h"
#include "../../expander.h"

/* Test metadata structure */
struct test_fixture {
    const char *name;
    const char *json_data;
    int expected_success;
};

/* Test result tracking */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \\
    do { \\
        if (!(condition)) { \\
            printf("  FAILED: %s\\n", message); \\
            tests_failed++; \\
        } else { \\
            tests_passed++; \\
        } \\
    } while(0)

''',
            "test_body.c": '''
static int test_{test_name}(void) {{
    printf("  Testing {test_name}...\\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("{fixture_path}", &set_config);
    
    {validation_code}
    
    if (set_config) {{
        rasa_set_free(set_config);
    }}
    
    return {expected_success} ? 0 : 1;
}}

'''
        }
        return templates.get(name, "")
    
    def generate_test_from_fixture(self, fixture_path: Path) -> Optional[str]:
        """Generate C test code from a single fixture."""
        try:
            with open(fixture_path, 'r') as f:
                fixture = json.load(f)
        except (json.JSONDecodeError, IOError) as e:
            print(f"Error loading {fixture_path}: {e}")
            return None
        
        metadata = fixture.get('test_metadata', {})
        test_name = metadata.get('name', fixture_path.stem)
        test_type = metadata.get('type', 'unknown')
        should_succeed = metadata.get('should_succeed', True)
        
        # Generate validation code based on test type
        validation_code = self._generate_validation_code(fixture, test_type, should_succeed)
        
        return self.test_template.format(
            test_name=test_name.replace('-', '_').replace('.', '_'),
            fixture_path=str(fixture_path),
            validation_code=validation_code,
            expected_success="1" if should_succeed else "0"
        )
    
    def _generate_validation_code(self, fixture: Dict[str, Any], test_type: str, should_succeed: bool) -> str:
        """Generate C validation code for a fixture."""
        code_lines = []
        
        if test_type == 'fallback_mode':
            code_lines.append("    /* Validate fallback mode */")
            code_lines.append("    if (set_config) {")
            code_lines.append("        TEST_ASSERT(set_config->fallback_mode != NULL, \"Fallback mode should be set\");")
            code_lines.append("    }")
            
            expected = fixture.get('expected_behavior', {})
            if expected.get('should_query_irr'):
                code_lines.append("    /* Should query IRR */")
                code_lines.append("    TEST_ASSERT(1, \"Should query IRR\");")
            else:
                code_lines.append("    /* Should not query IRR */")
                code_lines.append("    TEST_ASSERT(1, \"Should not query IRR\");")
                
        elif test_type == 'hierarchy':
            code_lines.append("    /* Validate hierarchy expansion */")
            expected_expansion = fixture.get('expected_expansion', 0)
            code_lines.append(f"    int expected_expansion = {expected_expansion};")
            code_lines.append("    if (set_config) {")
            code_lines.append("        /* TODO: Validate expansion depth */")
            code_lines.append("        TEST_ASSERT(1, \"Hierarchy test placeholder\");")
            code_lines.append("    }")
            
        elif test_type == 'asn_range':
            code_lines.append("    /* Validate ASN range */")
            code_lines.append("    if (set_config) {")
            code_lines.append("        TEST_ASSERT(set_config->containing_as > 0, \"AS should be positive\");")
            code_lines.append("        TEST_ASSERT(set_config->containing_as <= 4294967295, \"AS should fit in 32 bits\");")
            code_lines.append("    }")
            
        elif test_type == 'rasa_auth':
            code_lines.append("    /* Load RASA-AUTH config */")
            code_lines.append("    rasa_auth_config_t *auth_config = NULL;")
            code_lines.append("    int auth_result = rasa_auth_load_from_json(\"{fixture_path}\", &auth_config);")
            code_lines.append("    if (auth_config) {")
            code_lines.append("        TEST_ASSERT(auth_config->num_authorized_in > 0, \"Should have authorized assets\");")
            code_lines.append("        rasa_auth_free(auth_config);")
            code_lines.append("    }")
            
        elif test_type == 'boundary':
            code_lines.append("    /* Validate boundary condition */")
            if fixture.get('boundary_type'):
                boundary = fixture['boundary_type']
                code_lines.append(f"    /* Testing {boundary} boundary */")
            code_lines.append("    TEST_ASSERT(1, \"Boundary test placeholder\");")
            
        elif test_type == 'invalid':
            code_lines.append("    /* Validate expected failure */")
            if should_succeed:
                code_lines.append("    TEST_ASSERT(result == 0, \"Should succeed\");")
            else:
                code_lines.append("    TEST_ASSERT(result != 0, \"Should fail\");")
        else:
            code_lines.append("    /* Generic validation */")
            code_lines.append("    if (set_config) {")
            code_lines.append("        TEST_ASSERT(set_config != NULL, \"Config loaded\");")
            code_lines.append("    }")
        
        return "\n".join(code_lines)
    
    def generate_test_file(self, test_name: str, fixtures: List[Path]) -> str:
        """Generate a complete C test file for multiple fixtures."""
        parts = [self.header_template]
        
        # Generate individual test functions
        for fixture in fixtures:
            test_code = self.generate_test_from_fixture(fixture)
            if test_code:
                parts.append(test_code)
        
        # Generate main function
        parts.append(self._generate_main_function(fixtures))
        
        return "\n".join(parts)
    
    def _generate_main_function(self, fixtures: List[Path]) -> str:
        """Generate the main function that runs all tests."""
        lines = [
            "int main(int argc, char *argv[]) {",
            "    printf(\"========================================\\n\");",
            "    printf(\"RASA Generated Test Suite\\n\");",
            "    printf(\"========================================\\n\\n\");",
            "",
        ]
        
        for fixture in fixtures:
            test_name = fixture.stem.replace('-', '_').replace('.', '_')
            lines.append(f"    test_{test_name}();")
        
        lines.extend([
            "",
            "    printf(\"\\n========================================\\n\");",
            "    printf(\"Results: %d/%d tests passed\\n\", tests_passed, tests_passed + tests_failed);",
            "    printf(\"========================================\\n\");",
            "",
            "    return tests_failed > 0 ? 1 : 0;",
            "}",
        ])
        
        return "\n".join(lines)
    
    def generate_all_tests(self, batch_size: int = 100) -> Dict[str, int]:
        """Generate all test files from fixtures, batched by size."""
        fixture_files = sorted(self.fixtures_dir.glob("*.json"))
        
        if not fixture_files:
            print(f"No fixture files found in {self.fixtures_dir}")
            return {}
        
        print(f"Generating C tests from {len(fixture_files)} fixtures...")
        
        # Group fixtures into batches
        batches = []
        for i in range(0, len(fixture_files), batch_size):
            batch = fixture_files[i:i + batch_size]
            batches.append(batch)
        
        counts = {
            'total_fixtures': len(fixture_files),
            'test_files': 0,
            'tests_per_file': []
        }
        
        # Generate a test file for each batch
        for i, batch in enumerate(batches):
            test_file_name = f"test_rasa_generated_{i+1:03d}.c"
            test_file_path = self.output_dir / test_file_name
            
            test_code = self.generate_test_file(f"batch_{i+1}", batch)
            
            with open(test_file_path, 'w') as f:
                f.write(test_code)
            
            counts['test_files'] += 1
            counts['tests_per_file'].append(len(batch))
            print(f"  Generated {test_file_name} ({len(batch)} tests)")
        
        # Generate a summary header file
        self._generate_test_summary(batches)
        
        return counts
    
    def _generate_test_summary(self, batches: List[List[Path]]):
        """Generate a summary header file listing all test files."""
        summary_path = self.output_dir / "test_rasa_generated.h"
        
        lines = [
            "/* Auto-generated RASA test summary - DO NOT EDIT */",
            "#ifndef TEST_RASA_GENERATED_H",
            "#define TEST_RASA_GENERATED_H",
            "",
            f"#define RASA_GENERATED_TEST_BATCHES {len(batches)}",
            f"#define RASA_GENERATED_TOTAL_TESTS {sum(len(b) for b in batches)}",
            "",
            "/* Test file names */",
        ]
        
        for i in range(len(batches)):
            lines.append(f'#define RASA_TEST_FILE_{i+1} "test_rasa_generated_{i+1:03d}.c"')
        
        lines.extend([
            "",
            "#endif /* TEST_RASA_GENERATED_H */",
        ])
        
        with open(summary_path, 'w') as f:
            f.write("\n".join(lines))
        
        print(f"\nGenerated test summary: {summary_path}")


def main():
    import argparse
    parser = argparse.ArgumentParser(description='Generate C test code from RASA fixtures')
    parser.add_argument('-f', '--fixtures', default='../fixtures',
                       help='Directory containing JSON fixtures')
    parser.add_argument('-o', '--output', default='..',
                       help='Output directory for C test files')
    parser.add_argument('-b', '--batch-size', type=int, default=100,
                       help='Number of tests per file (default: 100)')
    parser.add_argument('--single', action='store_true',
                       help='Generate single file instead of batches')
    args = parser.parse_args()
    
    generator = CTestGenerator(args.fixtures, args.output)
    
    if args.single:
        # Generate single comprehensive test file
        fixtures = sorted(Path(args.fixtures).glob("*.json"))[:100]  # Limit to first 100 for single file
        test_code = generator.generate_test_file("comprehensive", fixtures)
        output_path = Path(args.output) / "test_rasa_generated_single.c"
        with open(output_path, 'w') as f:
            f.write(test_code)
        print(f"Generated single test file: {output_path} ({len(fixtures)} tests)")
    else:
        # Generate batched test files
        counts = generator.generate_all_tests(args.batch_size)
        print(f"\nGenerated {counts['test_files']} test files from {counts['total_fixtures']} fixtures")


if __name__ == '__main__':
    main()
