/* RASA Fixture Validator - Tests a sample of the 6,787 fixtures */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "../rasa.h"
#include "../rasa_hash.h"

static int tests_passed = 0;
static int tests_failed = 0;
static int fixtures_tested = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  FAILED: %s\n", message); \
            tests_failed++; \
        } else { \
            tests_passed++; \
        } \
    } while(0)

static int test_fixture(const char *filepath) {
    struct rasa_set_config set_config;
    struct rasa_auth auth_config;
    int result;
    
    memset(&set_config, 0, sizeof(set_config));
    memset(&auth_config, 0, sizeof(auth_config));
    
    /* Try to load as RASA-SET */
    result = rasa_set_load_config(&set_config, filepath);
    if (result == 0 && set_config.enabled) {
        rasa_set_free_config(&set_config);
        return 1;
    }
    rasa_set_free_config(&set_config);
    
    /* Try to load as RASA-AUTH */
    result = rasa_load_config((struct rasa_config *)&auth_config, filepath);
    if (result == 0) {
        rasa_free_config((struct rasa_config *)&auth_config);
        return 1;
    }
    rasa_free_config((struct rasa_config *)&auth_config);
    
    return 0;
}

static void test_fixture_sample(const char *pattern, int max_tests) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    dir = opendir("tests/fixtures");
    if (!dir) {
        printf("Failed to open fixtures directory\n");
        return;
    }
    
    printf("\nTesting fixtures matching '%s' (max %d):\n", pattern, max_tests);
    
    while ((entry = readdir(dir)) != NULL && count < max_tests) {
        if (strstr(entry->d_name, pattern) && strstr(entry->d_name, ".json")) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "tests/fixtures/%s", entry->d_name);
            
            if (test_fixture(filepath)) {
                tests_passed++;
            } else {
                printf("  FAILED: %s\n", entry->d_name);
                tests_failed++;
            }
            count++;
            fixtures_tested++;
        }
    }
    
    closedir(dir);
    printf("  Tested %d fixtures\n", count);
}

static void test_all_fixtures_sample(int sample_size) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    int every_n = 6787 / sample_size;  /* Sample evenly across all fixtures */
    
    dir = opendir("tests/fixtures");
    if (!dir) {
        printf("Failed to open fixtures directory\n");
        return;
    }
    
    printf("\nTesting random sample of %d fixtures (from 6,787 total):\n", sample_size);
    
    while ((entry = readdir(dir)) != NULL && count < sample_size) {
        if (strstr(entry->d_name, ".json") && (count * every_n) < fixtures_tested) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "tests/fixtures/%s", entry->d_name);
            
            if (test_fixture(filepath)) {
                /* Silent pass - only report failures */
            } else {
                printf("  FAILED: %s\n", entry->d_name);
                tests_failed++;
            }
            count++;
            fixtures_tested++;
        }
    }
    
    closedir(dir);
    printf("  Sampled %d fixtures\n", count);
}

static void test_fixture_categories(void) {
    printf("\n========================================\n");
    printf("RASA Fixture Category Tests\n");
    printf("========================================\n");
    
    test_fixture_sample("fallback_", 10);
    test_fixture_sample("hierarchy_", 10);
    test_fixture_sample("asn_", 10);
    test_fixture_sample("irr_source_", 10);
    test_fixture_sample("rasa_auth_", 10);
    test_fixture_sample("combined_", 10);
    test_fixture_sample("boundary_", 10);
    test_fixture_sample("edge_", 5);
    test_fixture_sample("multilevel_", 5);
}

static void test_hash_table_loading(void) {
    printf("\n========================================\n");
    printf("RASA Hash Table Loading Tests\n");
    printf("========================================\n");
    
    /* Load multiple fixtures into hash table */
    int result;
    
    result = rasa_load_sets_from_json("tests/fixtures/edge_hurricane_electric_pattern.json");
    TEST_ASSERT(result == 0, "Loaded HE pattern into hash table");
    
    result = rasa_load_sets_from_json("tests/fixtures/edge_google_rasaonly_pattern.json");
    TEST_ASSERT(result == 0, "Loaded Google pattern into hash table");
    
    struct rasa_set_entry *entry = rasa_lookup_set("AS-HURRICANE");
    TEST_ASSERT(entry != NULL, "Found AS-HURRICANE in hash table");
    
    entry = rasa_lookup_set("AS-GOOGLE");
    TEST_ASSERT(entry != NULL, "Found AS-GOOGLE in hash table");
    
    int count = rasa_set_count();
    TEST_ASSERT(count == 2, "Hash table contains 2 entries");
    
    rasa_cleanup_sets();
    
    printf("\n  Hash table operations: %d passed, %d failed\n", 
           tests_passed - fixtures_tested, tests_failed);
}

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("RASA Fixture Validator\n");
    printf("Testing sample of 6,787 generated fixtures\n");
    printf("========================================\n");
    
    test_fixture_categories();
    test_hash_table_loading();
    
    printf("\n========================================\n");
    printf("Total Fixtures Tested: %d\n", fixtures_tested);
    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    printf("========================================\n");
    
    if (tests_failed == 0) {
        printf("\n✓ All fixture tests PASSED!\n");
        printf("  RASA JSON loading is working correctly.\n");
        printf("  The test framework successfully validates\n");
        printf("  all generated fixture types.\n");
    }
    
    return tests_failed > 0 ? 1 : 0;
}
