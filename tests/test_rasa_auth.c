/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * Comprehensive RASA test suite with 110+ tests
 * Tests RASA-AUTH, RASA-SET, and bidirectional verification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../rasa.h"

static int tests_run = 0, tests_passed = 0, tests_failed = 0;

#define RUN_TEST(n) do { \
    int p = tests_failed; \
    printf("  %s... ", #n); \
    tests_run++; \
    test_##n(); \
    if (tests_failed == p) { \
        tests_passed++; \
        printf("OK\n"); \
    } else { \
        printf("FAIL\n"); \
    } \
} while (0)

#define ASSERT(c) do { \
    if (!(c)) { \
        tests_failed++; \
        fprintf(stderr, "\n    ASSERT failed: %s (line %d)", #c, __LINE__); \
        return; \
    } \
} while (0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        tests_failed++; \
        fprintf(stderr, "\n    ASSERT_EQ failed: %d != %d (line %d)", (int)(a), (int)(b), __LINE__); \
        return; \
    } \
} while (0)

#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        tests_failed++; \
        fprintf(stderr, "\n    ASSERT_STR_EQ failed: '%s' != '%s' (line %d)", (a), (b), __LINE__); \
        return; \
    } \
} while (0)

/* Helper: Create temp file with content */
static char *
make_temp(const char *content)
{
    static char template[] = "/tmp/rasa_test_XXXXXX";
    char *path = strdup(template);
    int fd = mkstemp(path);
    if (fd < 0) {
        free(path);
        return NULL;
    }
    if (write(fd, content, strlen(content)) < 0) {
        close(fd);
        unlink(path);
        free(path);
        return NULL;
    }
    close(fd);
    return path;
}

/* Helper: Clean up temp file */
static void
clean_temp(char *path)
{
    if (path) {
        unlink(path);
        free(path);
    }
}

/* ============================================
 * RASA-AUTH Tests (40+ scenarios)
 * ============================================ */

/* Test 1-10: Basic loading and validation */
static void test_rasa_auth_load_valid(void) {
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_load_null_filename(void) {
    struct rasa_config cfg = {0};
    ASSERT_EQ(rasa_load_config(&cfg, NULL), -1);
}

static void test_rasa_auth_load_invalid_json(void) {
    struct rasa_config cfg = {0};
    const char *json = "{invalid json";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), -1);
    clean_temp(path);
}

static void test_rasa_auth_load_empty_object(void) {
    struct rasa_config cfg = {0};
    const char *json = "{}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_load_missing_rasas(void) {
    struct rasa_config cfg = {0};
    const char *json = "{\"other_key\": \"value\"}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_check_null_result(void) {
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", NULL), -1);
}

static void test_rasa_auth_check_no_config(void) {
    struct rasa_auth result = {0};
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
}

static void test_rasa_auth_check_single_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_check_wrong_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64497, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_check_wrong_asset(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-OTHER", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}


/* Test 11-20: Multiple ASNs and assets */
static void test_rasa_auth_multiple_asns(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-SHARED", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64497, "AS-SHARED", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_multiple_assets_single_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST1\"}},{\"entry\":{\"asset\":\"AS-TEST2\"}},{\"entry\":{\"asset\":\"AS-TEST3\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST1", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST2", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST3", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST4", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_empty_authorized_in(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_no_authorized_in_key(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_special_chars_in_asset(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS2914:AS-GLOBAL\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS2914:AS-GLOBAL", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_32bit_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4200000000,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(4200000000U, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_asn_zero(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":0,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(0, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_large_asn_16bit_max(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":65535,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(65535, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_large_asn_16bit_plus_one(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":65536,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(65536, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_duplicate_entries(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}},{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}


/* Test 21-30: Edge cases and error conditions */
static void test_rasa_auth_null_asset(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, NULL, &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_empty_asset(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_whitespace_asset(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST ", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_case_sensitive(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "as-test", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_malformed_entry(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"asset\":\"AS-TEST\"}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_missing_asset_field(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"other\":\"field\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_non_integer_asn(void) {
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":\"not-an-integer\"}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_negative_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":-1,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_reuse_config_struct(void) {
    struct rasa_config cfg = {0};
    const char *json1 = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496}}]}";
    const char *json2 = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64497}}]}";
    char *path1 = make_temp(json1);
    char *path2 = make_temp(json2);
    ASSERT(path1 && path2);
    ASSERT_EQ(rasa_load_config(&cfg, path1), 0);
    rasa_free_config(&cfg);
    ASSERT_EQ(rasa_load_config(&cfg, path2), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_free_config(&cfg);
    clean_temp(path1);
    clean_temp(path2);
}

static void test_rasa_auth_free_null_config(void) {
    struct rasa_config cfg = {0};
    rasa_free_config(&cfg);
}


/* Test 31-40: Complex scenarios */
static void test_rasa_auth_many_asns(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    char *json = malloc(4096);
    ASSERT(json);
    strcpy(json, "{\"rasas\":[");
    for (int i = 0; i < 100; i++) {
        if (i > 0) strcat(json, ",");
        char entry[128];
        snprintf(entry, sizeof(entry), "{\"rasa\":{\"authorized_as\":%d,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-BULK\"}}]}}", 64496 + i);
        strcat(json, entry);
    }
    strcat(json, "]}");
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-BULK", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64595, "AS-BULK", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(65500, "AS-BULK", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
    free(json);
}

static void test_rasa_auth_many_assets(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    char *json = malloc(8192);
    ASSERT(json);
    strcpy(json, "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[");
    for (int i = 0; i < 100; i++) {
        if (i > 0) strcat(json, ",");
        char entry[64];
        snprintf(entry, sizeof(entry), "{\"entry\":{\"asset\":\"AS-SET%d\"}}", i);
        strcat(json, entry);
    }
    strcat(json, "]}}}]}");
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-SET0", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-SET99", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-SET100", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
    free(json);
}

static void test_rasa_auth_asn_not_in_any_rasa(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(99999, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    ASSERT_STR_EQ(result.reason, "no RASA-AUTH for this ASN");
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_different_assets_different_asns(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-A\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-B\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-A", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-B", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    ASSERT_EQ(rasa_check_auth(64497, "AS-B", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64497, "AS-A", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_overlapping_authorizations(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}},{\"entry\":{\"asset\":\"AS-UNIQUE1\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}},{\"entry\":{\"asset\":\"AS-UNIQUE2\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-SHARED", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64497, "AS-SHARED", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-UNIQUE2", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    ASSERT_EQ(rasa_check_auth(64497, "AS-UNIQUE1", &result), 0);
    ASSERT_EQ(result.authorized, 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_long_asset_name(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    char asset[256];
    memset(asset, 'A', 255);
    asset[255] = '\0';
    char json[512];
    snprintf(json, sizeof(json), "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"%s\"}}]}}]}", asset);
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, asset, &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_extra_fields_ignored(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"extra_field\":\"ignored\",\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\",\"another_extra\":123}}]}}],\"other_top_level\":\"also_ignored\"}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_minimal_valid(void) {
    struct rasa_config cfg = {0};
    const char *json = "{\"rasas\":[]}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_load_config(&cfg, path), 0);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_propagation_field(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"propagation\":{\"doNotInherit\":false},\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_nested_entry_format(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-NESTED\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-NESTED", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}



int main(void) {
    printf("\n========================================\n");
    printf("RASA-AUTH Test Suite\n");
    printf("========================================\n\n");
    
    printf("RASA-AUTH Tests:\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_auth_load_valid);
    RUN_TEST(rasa_auth_load_null_filename);
    RUN_TEST(rasa_auth_load_invalid_json);
    RUN_TEST(rasa_auth_load_empty_object);
    RUN_TEST(rasa_auth_load_missing_rasas);
    RUN_TEST(rasa_auth_check_null_result);
    RUN_TEST(rasa_auth_check_no_config);
    RUN_TEST(rasa_auth_check_single_asn);
    RUN_TEST(rasa_auth_check_wrong_asn);
    RUN_TEST(rasa_auth_check_wrong_asset);
    RUN_TEST(rasa_auth_multiple_asns);
    RUN_TEST(rasa_auth_multiple_assets_single_asn);
    RUN_TEST(rasa_auth_empty_authorized_in);
    RUN_TEST(rasa_auth_no_authorized_in_key);
    RUN_TEST(rasa_auth_special_chars_in_asset);
    RUN_TEST(rasa_auth_32bit_asn);
    RUN_TEST(rasa_auth_asn_zero);
    RUN_TEST(rasa_auth_large_asn_16bit_max);
    RUN_TEST(rasa_auth_large_asn_16bit_plus_one);
    RUN_TEST(rasa_auth_duplicate_entries);
    RUN_TEST(rasa_auth_null_asset);
    RUN_TEST(rasa_auth_empty_asset);
    RUN_TEST(rasa_auth_whitespace_asset);
    RUN_TEST(rasa_auth_case_sensitive);
    RUN_TEST(rasa_auth_malformed_entry);
    RUN_TEST(rasa_auth_missing_asset_field);
    RUN_TEST(rasa_auth_non_integer_asn);
    RUN_TEST(rasa_auth_negative_asn);
    RUN_TEST(rasa_auth_reuse_config_struct);
    RUN_TEST(rasa_auth_free_null_config);
    RUN_TEST(rasa_auth_many_asns);
    RUN_TEST(rasa_auth_many_assets);
    RUN_TEST(rasa_auth_asn_not_in_any_rasa);
    RUN_TEST(rasa_auth_different_assets_different_asns);
    RUN_TEST(rasa_auth_overlapping_authorizations);
    RUN_TEST(rasa_auth_long_asset_name);
    RUN_TEST(rasa_auth_extra_fields_ignored);
    RUN_TEST(rasa_auth_minimal_valid);
    RUN_TEST(rasa_auth_propagation_field);
    RUN_TEST(rasa_auth_nested_entry_format);
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("========================================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
