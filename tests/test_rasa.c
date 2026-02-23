/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * RASA unit tests - Minimal Viable Test Suite (Option C)
 * 20 critical tests for rasa.c functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "../rasa.h"

/* Test result tracking */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Test macros */
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

#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        printf("FAILED: Expected \"%s\", got \"%s\"\n", (b), (a)); \
        tests_failed++; \
        return; \
    } \
} while(0)

/* Helper: Create temp JSON file */
static char*
create_temp_json(const char *content)
{
    static char path[] = "/tmp/rasa_test_XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) return NULL;
    
    write(fd, content, strlen(content));
    close(fd);
    
    return strdup(path);
}

/* Helper: Cleanup temp file */
static void
cleanup_temp_json(char *path)
{
    if (path) {
        unlink(path);
        free(path);
    }
}

/* Test A1: Load valid JSON */
TEST(load_valid_json)
{
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-EXAMPLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    ASSERT(path != NULL);
    
    int rc = rasa_load_config(&cfg, path);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(cfg.enabled, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A2: Load invalid JSON */
TEST(load_invalid_json)
{
    struct rasa_config cfg = {0};
    const char *json = "{invalid json}";
    
    char *path = create_temp_json(json);
    ASSERT(path != NULL);
    
    int rc = rasa_load_config(&cfg, path);
    ASSERT_EQ(rc, -1);
    
    cleanup_temp_json(path);
}

/* Test A3: Load missing file */
TEST(load_missing_file)
{
    struct rasa_config cfg = {0};
    
    int rc = rasa_load_config(&cfg, "/nonexistent/path/rasa.json");
    ASSERT_EQ(rc, -1);
}

/* Test A4: Check authorized AS matches */
TEST(check_auth_authorized_asn)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-EXAMPLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(64496, "AS-EXAMPLE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_STR_EQ(result.reason, "authorized");
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A5: Check unauthorized AS */
TEST(check_auth_unauthorized_asn)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-EXAMPLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(64497, "AS-EXAMPLE", &result);
    ASSERT_EQ(rc, 0);
    /* Should allow by default since no RASA for AS64497 */
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A6: Check authorized in specific AS-SET */
TEST(check_auth_in_asset)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 15169, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-GOOGLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(15169, "AS-GOOGLE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A7: Check not authorized in different AS-SET */
TEST(check_auth_not_in_asset)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 15169, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-GOOGLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* AS15169 is not authorized in AS-EVIL */
    int rc = rasa_check_auth(15169, "AS-EVIL", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 0);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A8: Check with empty authorized_in */
TEST(check_auth_empty_authorized_in)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": []}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(64496, "AS-EXAMPLE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 0);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A9: Check with no RASA data (default allow) */
TEST(check_auth_no_rasa_data)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    
    /* Don't load any config */
    int rc = rasa_check_auth(64496, "AS-EXAMPLE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT(result.reason != NULL);
}

/* Test A10: Check with NULL asset */
TEST(check_auth_null_asset)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-EXAMPLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* With NULL asset, should still check authorized_in entries */
    int rc = rasa_check_auth(64496, NULL, &result);
    ASSERT_EQ(rc, 0);
    /* Won't match any specific AS-SET, so not authorized */
    ASSERT_EQ(result.authorized, 0);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A11: Free NULL config */
TEST(free_config_null)
{
    struct rasa_config cfg = {0};
    /* Should not crash */
    rasa_free_config(&cfg);
    ASSERT_EQ(cfg.enabled, 0);
}

/* Test A12: Free valid config */
TEST(free_config_valid)
{
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\": []}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    rasa_free_config(&cfg);
    ASSERT_EQ(cfg.enabled, 0);
    ASSERT(cfg.source_file == NULL);
    
    cleanup_temp_json(path);
}

/* Test A13: Multiple entries in authorized_in */
TEST(check_auth_multiple_entries)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 2914, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-NTT\"}}, {\"entry\": {\"asset\": \"AS-GLOBAL\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* Should match AS-NTT */
    int rc = rasa_check_auth(2914, "AS-NTT", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    /* Should match AS-GLOBAL */
    rc = rasa_check_auth(2914, "AS-GLOBAL", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    /* Should NOT match AS-OTHER */
    rc = rasa_check_auth(2914, "AS-OTHER", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 0);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A14: Case sensitivity */
TEST(check_auth_case_sensitive)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-EXAMPLE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* AS-EXAMPLE should match */
    int rc = rasa_check_auth(64496, "AS-EXAMPLE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    /* as-example should NOT match (case sensitive) */
    rc = rasa_check_auth(64496, "as-example", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 0);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A15: Large AS number (32-bit) */
TEST(check_auth_large_asn)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 4294967295, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-LARGE\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(4294967295U, "AS-LARGE", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A16: Zero AS number */
TEST(check_auth_zero_asn)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 0, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-ZERO\"}}]}}]}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    int rc = rasa_check_auth(0, "AS-ZERO", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A17: Load new file replaces old */
TEST(load_multiple_files)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    
    const char *json1 = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-OLD\"}}]}}]}";
    const char *json2 = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 15169, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-NEW\"}}]}}]}";
    
    char *path1 = create_temp_json(json1);
    char *path2 = create_temp_json(json2);
    
    rasa_load_config(&cfg, path1);
    
    /* Load second file - should replace first */
    rasa_load_config(&cfg, path2);
    
    /* Old AS should not be authorized anymore */
    int rc = rasa_check_auth(64496, "AS-OLD", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1); /* Default allow */
    
    /* New AS should be authorized */
    rc = rasa_check_auth(15169, "AS-NEW", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path1);
    cleanup_temp_json(path2);
}

/* Test A18: Empty rasas array */
TEST(load_empty_rasas)
{
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\": []}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    ASSERT_EQ(cfg.enabled, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A19: Missing rasas field */
TEST(load_missing_rasas)
{
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"other_field\": []}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* Should load but have no rasas */
    ASSERT_EQ(cfg.enabled, 1);
    
    /* Any AS should be allowed by default */
    int rc = rasa_check_auth(64496, "AS-TEST", &result);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(result.authorized, 1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Test A20: NULL result pointer */
TEST(check_auth_null_result)
{
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\": []}";
    
    char *path = create_temp_json(json);
    rasa_load_config(&cfg, path);
    
    /* Should return error with NULL result */
    int rc = rasa_check_auth(64496, "AS-TEST", NULL);
    ASSERT_EQ(rc, -1);
    
    rasa_free_config(&cfg);
    cleanup_temp_json(path);
}

/* Main test runner */
int
main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    printf("\n");
    printf("========================================\n");
    printf("RASA-AUTH Minimal Test Suite (Option C)\n");
    printf("========================================\n");
    printf("\n");
    
    printf("Running 20 unit tests for bgpq4/rasa.c:\n\n");
    
    /* Run all tests */
    RUN_TEST(load_valid_json);
    RUN_TEST(load_invalid_json);
    RUN_TEST(load_missing_file);
    RUN_TEST(check_auth_authorized_asn);
    RUN_TEST(check_auth_unauthorized_asn);
    RUN_TEST(check_auth_in_asset);
    RUN_TEST(check_auth_not_in_asset);
    RUN_TEST(check_auth_empty_authorized_in);
    RUN_TEST(check_auth_no_rasa_data);
    RUN_TEST(check_auth_null_asset);
    RUN_TEST(free_config_null);
    RUN_TEST(free_config_valid);
    RUN_TEST(check_auth_multiple_entries);
    RUN_TEST(check_auth_case_sensitive);
    RUN_TEST(check_auth_large_asn);
    RUN_TEST(check_auth_zero_asn);
    RUN_TEST(load_multiple_files);
    RUN_TEST(load_empty_rasas);
    RUN_TEST(load_missing_rasas);
    RUN_TEST(check_auth_null_result);
    
    printf("\n");
    printf("========================================\n");
    printf("Results: %d/%d passed, %d failed\n", 
           tests_passed, tests_run, tests_failed);
    printf("========================================\n");
    printf("\n");
    
    return tests_failed > 0 ? 1 : 0;
}
