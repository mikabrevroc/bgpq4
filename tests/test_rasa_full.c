/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * Full RASA test suite - 119 tests
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

static char* make_temp(const char* content)
{
    char *path = malloc(32);
    strcpy(path, "/tmp/r_XXXXXX");
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

static void clean_temp(char *path)
{
    if (path) {
        unlink(path);
        free(path);
    }
}

/* Test functions will be inserted here */

int main(void)
{
    printf("\nRASA Full Test Suite\n");
    printf("====================\n\n");
    
    /* RUN_TEST calls will be inserted here */
    
    printf("\n====================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("====================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
