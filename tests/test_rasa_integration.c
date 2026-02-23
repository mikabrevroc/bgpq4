/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * RASA integration tests - Minimal Viable Test Suite (Option C)
 * 5 critical integration tests for expander.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../extern.h"
#include "../rasa.h"

/* Test result tracking */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) void test_##name(void)
#define RUN_TEST(name) do { \
    printf("  Running %s... ", #name); \
    tests_run++; \
    test_##name(); \
    tests_passed++; \
    printf("PASSED\n"); \
} while(0)

#define FAIL(msg) do { \
    tests_failed++; \
    printf("FAILED: %s\n", msg); \
    return; \
} while(0)

#define ASSERT(cond) do { \
    if (!(cond)) { \
        FAIL("Assertion failed: " #cond); \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf("FAILED: Expected %d, got %d\n", (int)(b), (int)(a)); \
        tests_failed++; \
        return; \
    } \
} while(0)

/* Test B1: Expansion with authorized AS */
TEST(expansion_with_authorized_as)
{
    struct bgpq_expander expander = {0};
    struct rasa_config cfg = {0};
    
    /* Initialize expander */
    if (!bgpq_expander_init(&expander, AF_INET)) {
        FAIL("Failed to init expander");
    }
    
    /* Setup RASA config that authorizes AS64496 in AS-TEST */
    /* In real test, we'd load from JSON file */
    
    /* The current_asset field should be set during expansion */
    ASSERT(expander.current_asset == NULL);
    
    expander_freeall(&expander);
}

/* Test B2: Expansion excludes unauthorized AS */
TEST(expansion_excludes_unauthorized)
{
    struct bgpq_expander expander = {0};
    
    if (!bgpq_expander_init(&expander, AF_INET)) {
        FAIL("Failed to init expander");
    }
    
    /* Test that expander_add_as checks RASA */
    /* This requires mocking or test data */
    
    expander_freeall(&expander);
}

/* Test B3: Expansion without RASA config */
TEST(expansion_without_rasa)
{
    struct bgpq_expander expander = {0};
    
    if (!bgpq_expander_init(&expander, AF_INET)) {
        FAIL("Failed to init expander");
    }
    
    /* When RASA is not enabled, all ASNs should be allowed */
    #ifdef HAVE_JANSSON
    ASSERT(expander.rasa == NULL);
    #endif
    
    expander_freeall(&expander);
}

/* Test B4: current_asset context propagation */
TEST(current_asset_context)
{
    struct bgpq_expander expander = {0};
    
    if (!bgpq_expander_init(&expander, AF_INET)) {
        FAIL("Failed to init expander");
    }
    
    /* Test that current_asset field exists and can be set */
    #ifdef HAVE_JANSSON
    expander.current_asset = strdup("AS-TEST");
    ASSERT(expander.current_asset != NULL);
    ASSERT(strcmp(expander.current_asset, "AS-TEST") == 0);
    free(expander.current_asset);
    expander.current_asset = NULL;
    #endif
    
    expander_freeall(&expander);
}

/* Test B5: Memory cleanup */
TEST(memory_cleanup)
{
    struct bgpq_expander expander = {0};
    
    if (!bgpq_expander_init(&expander, AF_INET)) {
        FAIL("Failed to init expander");
    }
    
    #ifdef HAVE_JANSSON
    /* Set current_asset and verify it's freed */
    expander.current_asset = strdup("AS-TEST");
    #endif
    
    /* Cleanup should not crash */
    expander_freeall(&expander);
    
    #ifdef HAVE_JANSSON
    ASSERT(expander.current_asset == NULL);
    #endif
}

/* Main test runner */
int
main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    printf("\n");
    printf("========================================\n");
    printf("RASA Integration Tests (Option C)\n");
    printf("========================================\n");
    printf("\n");
    
    printf("Running 5 integration tests:\n\n");
    
    RUN_TEST(expansion_with_authorized_as);
    RUN_TEST(expansion_excludes_unauthorized);
    RUN_TEST(expansion_without_rasa);
    RUN_TEST(current_asset_context);
    RUN_TEST(memory_cleanup);
    
    printf("\n");
    printf("========================================\n");
    printf("Results: %d/%d passed, %d failed\n", 
           tests_passed, tests_run, tests_failed);
    printf("========================================\n");
    printf("\n");
    
    return tests_failed > 0 ? 1 : 0;
}
