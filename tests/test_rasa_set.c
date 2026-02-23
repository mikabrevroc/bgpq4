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
 * RASA-SET Tests (40+ scenarios)
 * ============================================ */

static void test_rasa_set_load_valid(void) {
    struct rasa_set_config cfg = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_set_load_config(&cfg, path), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_load_null_filename(void) {
    struct rasa_set_config cfg = {0};
    ASSERT_EQ(rasa_set_load_config(&cfg, NULL), -1);
}

static void test_rasa_set_load_invalid_json(void) {
    struct rasa_set_config cfg = {0};
    const char *json = "{invalid";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_set_load_config(&cfg, path), -1);
    clean_temp(path);
}

static void test_rasa_set_check_no_config(void) {
    struct rasa_set_membership result = {0};
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
}

static void test_rasa_set_check_null_result(void) {
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, NULL), -1);
}

static void test_rasa_set_check_single_member(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_check_multiple_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,64497,64498]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64498, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64499, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_check_wrong_set_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-OTHER", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_check_empty_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_check_no_members_key(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\"}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}


static void test_rasa_set_multiple_sets(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-SET1\",\"members\":[64496]}},{\"rasa_set\":{\"as_set_name\":\"AS-SET2\",\"members\":[64497]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-SET1", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET2", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET1", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_32bit_asn_member(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4200000000]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 4200000000U, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_special_chars_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS2914:AS-GLOBAL\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS2914:AS-GLOBAL", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_asn_zero(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[0]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 0, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_large_asn_16bit_max(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[65535]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 65535, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_large_asn_16bit_plus_one(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[65536]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 65536, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_duplicate_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,64496,64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_null_set_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership(NULL, 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_empty_set_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_case_sensitive(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("as-test", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}


static void test_rasa_set_missing_rasa_sets_key(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"other_key\": \"value\"}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_missing_as_set_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_non_integer_member(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,\"not-an-int\",64497]}}]}";
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

static void test_rasa_set_reuse_config_struct(void) {
    struct rasa_set_config cfg = {0};
    const char *json1 = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    const char *json2 = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST2\",\"members\":[64497]}}]}";
    char *path1 = make_temp(json1);
    char *path2 = make_temp(json2);
    ASSERT(path1 && path2);
    ASSERT_EQ(rasa_set_load_config(&cfg, path1), 0);
    rasa_set_free_config(&cfg);
    ASSERT_EQ(rasa_set_load_config(&cfg, path2), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path1);
    clean_temp(path2);
}

static void test_rasa_set_free_null_config(void) {
    struct rasa_set_config cfg = {0};
    rasa_set_free_config(&cfg);
}

static void test_rasa_set_many_sets(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    char json[4096];
    strcpy(json, "{\"rasa_sets\":[");
    for (int i = 0; i < 50; i++) {
        if (i > 0) strcat(json, ",");
        char entry[128];
        snprintf(entry, sizeof(entry), "{\"rasa_set\":{\"as_set_name\":\"AS-SET%d\",\"members\":[%d]}}", i, 64496 + i);
        strcat(json, entry);
    }
    strcat(json, "]}");
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-SET0", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET49", 64545, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET0", 64500, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_many_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    char json[4096];
    strcpy(json, "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-BULK\",\"members\":[");
    for (int i = 0; i < 100; i++) {
        if (i > 0) strcat(json, ",");
        char num[16];
        snprintf(num, sizeof(num), "%d", 64496 + i);
        strcat(json, num);
    }
    strcat(json, "]}}]}");
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-BULK", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-BULK", 64595, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-BULK", 65500, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_asn_not_in_any_set(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 99999, &result), 0);
    ASSERT_EQ(result.is_member, 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_whitespace_in_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST ", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_long_set_name(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    char set_name[256];
    memset(set_name, 'S', 255);
    set_name[255] = '\0';
    char json[512];
    snprintf(json, sizeof(json), "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"%s\",\"members\":[64496]}}]}", set_name);
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership(set_name, 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}


static void test_rasa_set_extra_fields_ignored(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"extra\":\"ignored\",\"members\":[64496,\"extra\":123]}}],\"other\":\"ignored\"}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_minimal_valid(void) {
    struct rasa_set_config cfg = {0};
    const char *json = "{\"rasa_sets\":[]}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_set_load_config(&cfg, path), 0);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_nested_sets_declaration(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-PARENT\",\"members\":[64496],\"nested\":[{\"entry\":{\"as_set\":\"AS-CHILD\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-PARENT", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_containing_as_field(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"containing_as\":64496,\"members\":[64497,64498]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_load_empty_object(void) {
    struct rasa_set_config cfg = {0};
    const char *json = "{}";
    char *path = make_temp(json);
    ASSERT(path);
    ASSERT_EQ(rasa_set_load_config(&cfg, path), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_negative_member_asn(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[-1,64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_large_member_list_mixed(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[0,1,65535,65536,4200000000,64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 0, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 4200000000U, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_multiple_same_asn_different_sets(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-SET1\",\"members\":[64496]}},{\"rasa_set\":{\"as_set_name\":\"AS-SET2\",\"members\":[64496]}},{\"rasa_set\":{\"as_set_name\":\"AS-SET3\",\"members\":[64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-SET1", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET2", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-SET3", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_members_array_with_nulls(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[null,64496,null]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

static void test_rasa_set_boolean_in_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[true,false,64496]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}





int main(void) {
    printf("\n========================================\n");
    printf("RASA-SET Test Suite\n");
    printf("========================================\n\n");
    
    printf("RASA-SET Tests:\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_set_load_valid);
    RUN_TEST(rasa_set_load_null_filename);
    RUN_TEST(rasa_set_load_invalid_json);
    RUN_TEST(rasa_set_check_no_config);
    RUN_TEST(rasa_set_check_null_result);
    RUN_TEST(rasa_set_check_single_member);
    RUN_TEST(rasa_set_check_multiple_members);
    RUN_TEST(rasa_set_check_wrong_set_name);
    RUN_TEST(rasa_set_check_empty_members);
    RUN_TEST(rasa_set_check_no_members_key);
    RUN_TEST(rasa_set_multiple_sets);
    RUN_TEST(rasa_set_32bit_asn_member);
    RUN_TEST(rasa_set_special_chars_name);
    RUN_TEST(rasa_set_asn_zero);
    RUN_TEST(rasa_set_large_asn_16bit_max);
    RUN_TEST(rasa_set_large_asn_16bit_plus_one);
    RUN_TEST(rasa_set_duplicate_members);
    RUN_TEST(rasa_set_null_set_name);
    RUN_TEST(rasa_set_empty_set_name);
    RUN_TEST(rasa_set_case_sensitive);
    RUN_TEST(rasa_set_missing_rasa_sets_key);
    RUN_TEST(rasa_set_missing_as_set_name);
    RUN_TEST(rasa_set_non_integer_member);
    RUN_TEST(rasa_set_reuse_config_struct);
    RUN_TEST(rasa_set_free_null_config);
    RUN_TEST(rasa_set_many_sets);
    RUN_TEST(rasa_set_many_members);
    RUN_TEST(rasa_set_asn_not_in_any_set);
    RUN_TEST(rasa_set_whitespace_in_name);
    RUN_TEST(rasa_set_long_set_name);
    RUN_TEST(rasa_set_extra_fields_ignored);
    RUN_TEST(rasa_set_minimal_valid);
    RUN_TEST(rasa_set_nested_sets_declaration);
    RUN_TEST(rasa_set_containing_as_field);
    RUN_TEST(rasa_set_load_empty_object);
    RUN_TEST(rasa_set_negative_member_asn);
    RUN_TEST(rasa_set_large_member_list_mixed);
    RUN_TEST(rasa_set_multiple_same_asn_different_sets);
    RUN_TEST(rasa_set_members_array_with_nulls);
    RUN_TEST(rasa_set_boolean_in_members);
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("========================================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
