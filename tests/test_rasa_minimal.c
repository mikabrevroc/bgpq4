/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * Minimal RASA test - just 3 tests to verify build
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
        return; \
    } \
} while (0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        tests_failed++; \
        return; \
    } \
} while (0)

static char* make_temp(const char*c)
{
    static char p[] = "/tmp/r_XXXXXX";
    int fd = mkstemp(p);
    if (fd < 0) return NULL;
    write(fd, c, strlen(c));
    close(fd);
    return strdup(p);
}

static void clean_temp(char*p)
{
    if (p) {
        unlink(p);
        free(p);
    }
}

static void test_load_valid(void)
{
    struct rasa_config cfg = {0};
    const char *j = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496}}]}";
    char *p = make_temp(j);
    ASSERT(p);
    ASSERT_EQ(rasa_load_config(&cfg, p), 0);
    ASSERT_EQ(cfg.enabled, 1);
    rasa_free_config(&cfg);
    clean_temp(p);
}

static void test_check_auth(void)
{
    struct rasa_config cfg = {0};
    struct rasa_auth r = {0};
    const char *j = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-T\"}}]}}]}";
    char *p = make_temp(j);
    rasa_load_config(&cfg, p);
    ASSERT_EQ(rasa_check_auth(64496, "AS-T", &r), 0);
    ASSERT_EQ(r.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(p);
}

static void test_bidirectional(void)
{
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* No configs - should default allow */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
}

int main(void)
{
    printf("\nRASA Minimal Test Suite\n");
    printf("========================\n\n");
    
    RUN_TEST(load_valid);
    RUN_TEST(check_auth);
    RUN_TEST(bidirectional);
    
    printf("\n========================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("========================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
