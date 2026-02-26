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
 * ============================================ */

static void test_rasa_auth_very_large_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Maximum 32-bit unsigned value */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4294967295,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(4294967295U, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_very_large_asn(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4294967295]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 4294967295U, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_private_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Private ASN range */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64512,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64512, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_private_asn(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64512,65534]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64512, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 65534, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_reserved_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* ASN 23456 is reserved for AS_TRANS */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":23456,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(23456, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_auth_nested_arrays(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Test handling of nested arrays that might cause issues */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}},{\"entry\":{\"asset\":\"AS-TEST2\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST2", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_mixed_types_in_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    /* Test that non-integer members are skipped gracefully */
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,\"string\",true,false,null,64497,[],{}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_bidirectional_very_large_asn(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4294967295,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4294967295]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 4294967295U, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

static void test_bidirectional_private_asns(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64512,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64512]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64512, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

static void test_bidirectional_as_trans(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":23456,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[23456]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 23456, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


/* ============================================
 * Main Test Runner
 * ============================================ */



int main(void) {
    printf("\n========================================\n");
    printf("RASA Edge Case Tests\n");
    printf("========================================\n\n");
    
    printf("Edge Case Tests:\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_auth_very_large_asn);
    RUN_TEST(rasa_set_very_large_asn);
    RUN_TEST(rasa_auth_private_asn);
    RUN_TEST(rasa_set_private_asn);
    RUN_TEST(rasa_auth_reserved_asn);
    RUN_TEST(rasa_auth_nested_arrays);
    RUN_TEST(rasa_set_mixed_types_in_members);
    RUN_TEST(bidirectional_very_large_asn);
    RUN_TEST(bidirectional_private_asns);
    RUN_TEST(bidirectional_as_trans);
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("========================================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
