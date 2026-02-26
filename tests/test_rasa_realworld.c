/* RASA Real-World Scenario Tests */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../rasa.h"
#include "../rasa_hash.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  FAILED: %s\n", message); \
            tests_failed++; \
        } else { \
            printf("  OK: %s\n", message); \
            tests_passed++; \
        } \
    } while(0)

static void test_hurricane_electric(void) {
    printf("\nTest: Hurricane Electric AS-HURRICANE\n");
    
    struct rasa_set_config config;
    memset(&config, 0, sizeof(config));
    
    int result = rasa_set_load_config(&config, "tests/fixtures/sample/edge_hurricane_electric_pattern.json");
    
    TEST_ASSERT(result == 0, "HE config loaded");
    TEST_ASSERT(config.enabled == 1, "Config enabled");
    TEST_ASSERT(config.fallback_mode == RASA_FALLBACK_MODE_IRR_LOCK, "irrLock mode");
    TEST_ASSERT(config.irr_source != NULL, "IRR source set");
    
    rasa_set_free_config(&config);
}

static void test_google(void) {
    printf("\nTest: Google AS-GOOGLE\n");
    
    struct rasa_set_config config;
    memset(&config, 0, sizeof(config));
    
    int result = rasa_set_load_config(&config, "tests/fixtures/sample/edge_google_rasaonly_pattern.json");
    
    TEST_ASSERT(result == 0, "Google config loaded");
    TEST_ASSERT(config.enabled == 1, "Config enabled");
    TEST_ASSERT(config.fallback_mode == RASA_FALLBACK_MODE_RASA_ONLY, "rasaOnly mode");
    
    rasa_set_free_config(&config);
}

static void test_he_membership(void) {
    printf("\nTest: HE Membership Check\n");
    
    struct rasa_set_membership membership;
    memset(&membership, 0, sizeof(membership));
    
    int result = rasa_check_set_membership("AS-HURRICANE", 7922, &membership);
    
    TEST_ASSERT(result == 0, "Membership check executed");
}

static void test_multilevel(void) {
    printf("\nTest: Multi-level Hierarchy\n");
    
    struct rasa_set_config config;
    memset(&config, 0, sizeof(config));
    
    int result = rasa_set_load_config(&config, "tests/fixtures/sample/hierarchy_2level_irrFallback_1.json");
    
    TEST_ASSERT(result == 0, "Multi-level config loaded");
    TEST_ASSERT(config.enabled == 1, "Config enabled");
    
    rasa_set_free_config(&config);
}

static void test_strict_auth(void) {
    printf("\nTest: Strict Mode Authorization\n");
    
    struct rasa_auth auth_result;
    memset(&auth_result, 0, sizeof(auth_result));
    
    int result = rasa_check_auth(64499, "AS-TIER1", &auth_result);
    
    TEST_ASSERT(result == 0, "Auth check executed");
}

static void test_bidirectional(void) {
    printf("\nTest: Bidirectional Verification\n");
    
    struct rasa_auth auth_result;
    struct rasa_set_membership set_result;
    memset(&auth_result, 0, sizeof(auth_result));
    memset(&set_result, 0, sizeof(set_result));
    
    int result = rasa_verify_bidirectional("AS-TEST", 64497, &auth_result, &set_result);
    
    TEST_ASSERT(result == 0, "Bidirectional check executed");
}

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("RASA Real-World Scenario Tests\n");
    printf("========================================\n");
    
    test_hurricane_electric();
    test_google();
    test_he_membership();
    test_multilevel();
    test_strict_auth();
    test_bidirectional();
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_passed + tests_failed);
    printf("========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
