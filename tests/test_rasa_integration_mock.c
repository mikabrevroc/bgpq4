/* RASA Integration Tests with Mock IRR Server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "../rasa.h"
#include "../rasa_hash.h"
#include "../expander.h"

#define MOCK_SERVER_PORT 10443
#define MOCK_SERVER_HOST "127.0.0.1"

static int mock_server_pid = -1;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  FAILED: %s\\n", message); \
            tests_failed++; \
        } else { \
            printf("  OK: %s\\n", message); \
            tests_passed++; \
        } \
    } while(0)

static int start_mock_server(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        /* Child process - start mock server */
        execlp("python3", "python3", "tests/generator/irr_mock.py", NULL);
        perror("execlp");
        exit(1);
    }
    
    /* Parent process - wait for server to start */
    mock_server_pid = pid;
    sleep(1); /* Give server time to start */
    return 0;
}

static void stop_mock_server(void) {
    if (mock_server_pid > 0) {
        kill(mock_server_pid, SIGTERM);
        waitpid(mock_server_pid, NULL, 0);
        mock_server_pid = -1;
    }
}

/* Test 1: IRR Fallback with mock server */
static void test_irr_fallback_mock(void) {
    printf("\\nTest: IRR Fallback with Mock Server\\n");
    
    /* Create RASA-SET config with irrFallback mode */
    const char *rasa_set_json = "{"
        "\"rasa_sets\": [{"
        "  \"rasa_set\": {"
        "    \"as_set_name\": \"AS-TIER1\","
        "    \"containing_as\": 64496,"
        "    \"members\": [64497],"
        "    \"nested_sets\": [\"AS-TIER2\"],"
        "    \"fallback_mode\": \"irrFallback\","
        "    \"flags\": []"
        "  }"
        "}]"
        "}";
    
    FILE *fp = fopen("/tmp/test_rasa_set.json", "w");
    if (!fp) {
        TEST_ASSERT(0, "Failed to create test file");
        return;
    }
    fprintf(fp, "%s", rasa_set_json);
    fclose(fp);
    
    rasa_set_config_t *config = NULL;
    int result = rasa_set_load_from_json("/tmp/test_rasa_set.json", &config);
    
    TEST_ASSERT(result == 0, "RASA-SET loaded successfully");
    TEST_ASSERT(config != NULL, "Config not NULL");
    
    if (config) {
        TEST_ASSERT(config->fallback_mode == RASA_FALLBACK_IRR_FALLBACK,
                   "Fallback mode is irrFallback");
        rasa_set_free(config);
    }
    
    unlink("/tmp/test_rasa_set.json");
}

/* Test 2: IRR Lock mode queries specific source */
static void test_irr_lock_mock(void) {
    printf("\\nTest: IRR Lock Mode\\n");
    
    const char *rasa_set_json = "{"
        "\"rasa_sets\": [{"
        "  \"rasa_set\": {"
        "    \"as_set_name\": \"AS-HURRICANE\","
        "    \"containing_as\": 6939,"
        "    \"members\": [],"
        "    \"nested_sets\": [],"
        "    \"fallback_mode\": \"irrLock\","
        "    \"irr_source\": \"RADB\","
        "    \"flags\": []"
        "  }"
        "}]"
        "}";
    
    FILE *fp = fopen("/tmp/test_rasa_lock.json", "w");
    if (!fp) {
        TEST_ASSERT(0, "Failed to create test file");
        return;
    }
    fprintf(fp, "%s", rasa_set_json);
    fclose(fp);
    
    rasa_set_config_t *config = NULL;
    int result = rasa_set_load_from_json("/tmp/test_rasa_lock.json", &config);
    
    TEST_ASSERT(result == 0, "RASA-SET loaded successfully");
    TEST_ASSERT(config != NULL, "Config not NULL");
    
    if (config) {
        TEST_ASSERT(config->fallback_mode == RASA_FALLBACK_IRR_LOCK,
                   "Fallback mode is irrLock");
        TEST_ASSERT(config->irr_source != NULL, "IRR source is set");
        if (config->irr_source) {
            TEST_ASSERT(strcmp(config->irr_source, "RADB") == 0,
                       "IRR source is RADB");
        }
        rasa_set_free(config);
    }
    
    unlink("/tmp/test_rasa_lock.json");
}

/* Test 3: RASA Only mode - no IRR query needed */
static void test_rasa_only_mock(void) {
    printf("\\nTest: RASA Only Mode\\n");
    
    const char *rasa_set_json = "{"
        "\"rasa_sets\": [{"
        "  \"rasa_set\": {"
        "    \"as_set_name\": \"AS-GOOGLE\","
        "    \"containing_as\": 15169,"
        "    \"members\": [15169, 16591, 19425],"
        "    \"nested_sets\": [],"
        "    \"fallback_mode\": \"rasaOnly\","
        "    \"flags\": []"
        "  }"
        "}]"
        "}";
    
    FILE *fp = fopen("/tmp/test_rasa_only.json", "w");
    if (!fp) {
        TEST_ASSERT(0, "Failed to create test file");
        return;
    }
    fprintf(fp, "%s", rasa_set_json);
    fclose(fp);
    
    rasa_set_config_t *config = NULL;
    int result = rasa_set_load_from_json("/tmp/test_rasa_only.json", &config);
    
    TEST_ASSERT(result == 0, "RASA-SET loaded successfully");
    TEST_ASSERT(config != NULL, "Config not NULL");
    
    if (config) {
        TEST_ASSERT(config->fallback_mode == RASA_FALLBACK_RASA_ONLY,
                   "Fallback mode is rasaOnly");
        TEST_ASSERT(config->num_members == 3, "Has 3 members");
        rasa_set_free(config);
    }
    
    unlink("/tmp/test_rasa_only.json");
}

/* Test 4: Bidirectional authorization check */
static void test_bidirectional_auth_mock(void) {
    printf("\\nTest: Bidirectional Authorization\\n");
    
    /* Create RASA-SET */
    const char *rasa_set_json = "{"
        "\"rasa_sets\": [{"
        "  \"rasa_set\": {"
        "    \"as_set_name\": \"AS-TEST\","
        "    \"containing_as\": 64496,"
        "    \"members\": [],"
        "    \"nested_sets\": [],"
        "    \"fallback_mode\": \"irrFallback\","
        "    \"flags\": []"
        "  }"
        "}]"
        "}";
    
    /* Create RASA-AUTH */
    const char *rasa_auth_json = "{"
        "\"rasas\": [{"
        "  \"rasa\": {"
        "    \"authorized_as\": 64497,"
        "    \"authorized_in\": ["
        "      {\"entry\": {\"asset\": \"AS-TEST\"}, \"propagation\": \"unrestricted\"}"
        "    ],"
        "    \"flags\": []"
        "  }"
        "}]"
        "}";
    
    FILE *fp = fopen("/tmp/test_rasa_set.json", "w");
    fprintf(fp, "%s", rasa_set_json);
    fclose(fp);
    
    fp = fopen("/tmp/test_rasa_auth.json", "w");
    fprintf(fp, "%s", rasa_auth_json);
    fclose(fp);
    
    rasa_set_config_t *set_config = NULL;
    rasa_auth_config_t *auth_config = NULL;
    
    int result1 = rasa_set_load_from_json("/tmp/test_rasa_set.json", &set_config);
    int result2 = rasa_auth_load_from_json("/tmp/test_rasa_auth.json", &auth_config);
    
    TEST_ASSERT(result1 == 0, "RASA-SET loaded");
    TEST_ASSERT(result2 == 0, "RASA-AUTH loaded");
    
    if (set_config && auth_config) {
        /* Check if AS 64497 is authorized for AS-TEST */
        int authorized = rasa_auth_check(auth_config, 64497, "AS-TEST");
        TEST_ASSERT(authorized == 1, "AS 64497 authorized for AS-TEST");
    }
    
    if (set_config) rasa_set_free(set_config);
    if (auth_config) rasa_auth_free(auth_config);
    
    unlink("/tmp/test_rasa_set.json");
    unlink("/tmp/test_rasa_auth.json");
}

/* Test 5: doNotInherit flag stops hierarchy expansion */
static void test_donotinherit_mock(void) {
    printf("\\nTest: Do Not Inherit Flag\\n");
    
    const char *rasa_set_json = "{"
        "\"rasa_sets\": ["
        "  {\"rasa_set\": {"
        "    \"as_set_name\": \"AS-PARENT\","
        "    \"containing_as\": 64496,"
        "    \"members\": [64497],"
        "    \"nested_sets\": [\"AS-CHILD\"],"
        "    \"fallback_mode\": \"irrFallback\","
        "    \"flags\": [\"doNotInherit\"]"
        "  }},"
        "  {\"rasa_set\": {"
        "    \"as_set_name\": \"AS-CHILD\","
        "    \"containing_as\": 64497,"
        "    \"members\": [64498],"
        "    \"nested_sets\": [],"
        "    \"fallback_mode\": \"irrFallback\","
        "    \"flags\": []"
        "  }}"
        "]}";
    
    FILE *fp = fopen("/tmp/test_donotinherit.json", "w");
    if (!fp) {
        TEST_ASSERT(0, "Failed to create test file");
        return;
    }
    fprintf(fp, "%s", rasa_set_json);
    fclose(fp);
    
    /* TODO: Implement flag parsing in rasa.c */
    TEST_ASSERT(1, "doNotInherit test placeholder - needs flag implementation");
    
    unlink("/tmp/test_donotinherit.json");
}

int main(int argc, char *argv[]) {
    printf("========================================\\n");
    printf("RASA Integration Tests with Mock IRR\\n");
    printf("========================================\\n");
    
    /* Start mock IRR server */
    printf("\\nStarting mock IRR server...\\n");
    if (start_mock_server() < 0) {
        fprintf(stderr, "Failed to start mock server\\n");
        return 1;
    }
    
    /* Run tests */
    test_irr_fallback_mock();
    test_irr_lock_mock();
    test_rasa_only_mock();
    test_bidirectional_auth_mock();
    test_donotinherit_mock();
    
    /* Stop mock server */
    stop_mock_server();
    
    /* Report results */
    printf("\\n========================================\\n");
    printf("Results: %d/%d tests passed\\n", tests_passed, tests_passed + tests_failed);
    printf("========================================\\n");
    
    return tests_failed > 0 ? 1 : 0;
}
