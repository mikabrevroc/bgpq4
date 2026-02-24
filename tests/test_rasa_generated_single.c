/* Auto-generated RASA test file - DO NOT EDIT */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../rasa.h"
#include "../../rasa_hash.h"
#include "../../expander.h"

/* Test metadata structure */
struct test_fixture {
    const char *name;
    const char *json_data;
    int expected_success;
};

/* Test result tracking */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  FAILED: %s\n", message); \
            tests_failed++; \
        } else { \
            tests_passed++; \
        } \
    } while(0)



static int test_asn_16bit_documentation_64496_irrFallback_authoritative_2402(void) {
    printf("  Testing asn_16bit_documentation_64496_irrFallback_authoritative_2402...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrFallback_authoritative_2402.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrFallback_doNotInherit+authoritative_2404(void) {
    printf("  Testing asn_16bit_documentation_64496_irrFallback_doNotInherit+authoritative_2404...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrFallback_doNotInherit+authoritative_2404.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrFallback_doNotInherit_2403(void) {
    printf("  Testing asn_16bit_documentation_64496_irrFallback_doNotInherit_2403...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrFallback_doNotInherit_2403.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrFallback_none_2401(void) {
    printf("  Testing asn_16bit_documentation_64496_irrFallback_none_2401...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrFallback_none_2401.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrLock_authoritative_2406(void) {
    printf("  Testing asn_16bit_documentation_64496_irrLock_authoritative_2406...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrLock_authoritative_2406.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrLock_doNotInherit+authoritative_2408(void) {
    printf("  Testing asn_16bit_documentation_64496_irrLock_doNotInherit+authoritative_2408...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrLock_doNotInherit+authoritative_2408.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrLock_doNotInherit_2407(void) {
    printf("  Testing asn_16bit_documentation_64496_irrLock_doNotInherit_2407...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrLock_doNotInherit_2407.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_irrLock_none_2405(void) {
    printf("  Testing asn_16bit_documentation_64496_irrLock_none_2405...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_irrLock_none_2405.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_rasaOnly_authoritative_2410(void) {
    printf("  Testing asn_16bit_documentation_64496_rasaOnly_authoritative_2410...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_rasaOnly_authoritative_2410.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_rasaOnly_doNotInherit+authoritative_2412(void) {
    printf("  Testing asn_16bit_documentation_64496_rasaOnly_doNotInherit+authoritative_2412...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_rasaOnly_doNotInherit+authoritative_2412.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_rasaOnly_doNotInherit_2411(void) {
    printf("  Testing asn_16bit_documentation_64496_rasaOnly_doNotInherit_2411...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_rasaOnly_doNotInherit_2411.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64496_rasaOnly_none_2409(void) {
    printf("  Testing asn_16bit_documentation_64496_rasaOnly_none_2409...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64496_rasaOnly_none_2409.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrFallback_authoritative_2414(void) {
    printf("  Testing asn_16bit_documentation_64497_irrFallback_authoritative_2414...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrFallback_authoritative_2414.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrFallback_doNotInherit+authoritative_2416(void) {
    printf("  Testing asn_16bit_documentation_64497_irrFallback_doNotInherit+authoritative_2416...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrFallback_doNotInherit+authoritative_2416.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrFallback_doNotInherit_2415(void) {
    printf("  Testing asn_16bit_documentation_64497_irrFallback_doNotInherit_2415...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrFallback_doNotInherit_2415.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrFallback_none_2413(void) {
    printf("  Testing asn_16bit_documentation_64497_irrFallback_none_2413...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrFallback_none_2413.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrLock_authoritative_2418(void) {
    printf("  Testing asn_16bit_documentation_64497_irrLock_authoritative_2418...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrLock_authoritative_2418.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrLock_doNotInherit+authoritative_2420(void) {
    printf("  Testing asn_16bit_documentation_64497_irrLock_doNotInherit+authoritative_2420...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrLock_doNotInherit+authoritative_2420.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrLock_doNotInherit_2419(void) {
    printf("  Testing asn_16bit_documentation_64497_irrLock_doNotInherit_2419...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrLock_doNotInherit_2419.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_irrLock_none_2417(void) {
    printf("  Testing asn_16bit_documentation_64497_irrLock_none_2417...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_irrLock_none_2417.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_rasaOnly_authoritative_2422(void) {
    printf("  Testing asn_16bit_documentation_64497_rasaOnly_authoritative_2422...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_rasaOnly_authoritative_2422.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_rasaOnly_doNotInherit+authoritative_2424(void) {
    printf("  Testing asn_16bit_documentation_64497_rasaOnly_doNotInherit+authoritative_2424...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_rasaOnly_doNotInherit+authoritative_2424.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_rasaOnly_doNotInherit_2423(void) {
    printf("  Testing asn_16bit_documentation_64497_rasaOnly_doNotInherit_2423...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_rasaOnly_doNotInherit_2423.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64497_rasaOnly_none_2421(void) {
    printf("  Testing asn_16bit_documentation_64497_rasaOnly_none_2421...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64497_rasaOnly_none_2421.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrFallback_authoritative_2426(void) {
    printf("  Testing asn_16bit_documentation_64498_irrFallback_authoritative_2426...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrFallback_authoritative_2426.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrFallback_doNotInherit+authoritative_2428(void) {
    printf("  Testing asn_16bit_documentation_64498_irrFallback_doNotInherit+authoritative_2428...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrFallback_doNotInherit+authoritative_2428.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrFallback_doNotInherit_2427(void) {
    printf("  Testing asn_16bit_documentation_64498_irrFallback_doNotInherit_2427...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrFallback_doNotInherit_2427.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrFallback_none_2425(void) {
    printf("  Testing asn_16bit_documentation_64498_irrFallback_none_2425...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrFallback_none_2425.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrLock_authoritative_2430(void) {
    printf("  Testing asn_16bit_documentation_64498_irrLock_authoritative_2430...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrLock_authoritative_2430.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrLock_doNotInherit+authoritative_2432(void) {
    printf("  Testing asn_16bit_documentation_64498_irrLock_doNotInherit+authoritative_2432...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrLock_doNotInherit+authoritative_2432.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrLock_doNotInherit_2431(void) {
    printf("  Testing asn_16bit_documentation_64498_irrLock_doNotInherit_2431...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrLock_doNotInherit_2431.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_irrLock_none_2429(void) {
    printf("  Testing asn_16bit_documentation_64498_irrLock_none_2429...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_irrLock_none_2429.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_rasaOnly_authoritative_2434(void) {
    printf("  Testing asn_16bit_documentation_64498_rasaOnly_authoritative_2434...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_rasaOnly_authoritative_2434.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_rasaOnly_doNotInherit+authoritative_2436(void) {
    printf("  Testing asn_16bit_documentation_64498_rasaOnly_doNotInherit+authoritative_2436...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_rasaOnly_doNotInherit+authoritative_2436.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_rasaOnly_doNotInherit_2435(void) {
    printf("  Testing asn_16bit_documentation_64498_rasaOnly_doNotInherit_2435...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_rasaOnly_doNotInherit_2435.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64498_rasaOnly_none_2433(void) {
    printf("  Testing asn_16bit_documentation_64498_rasaOnly_none_2433...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64498_rasaOnly_none_2433.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrFallback_authoritative_2438(void) {
    printf("  Testing asn_16bit_documentation_64499_irrFallback_authoritative_2438...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrFallback_authoritative_2438.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrFallback_doNotInherit+authoritative_2440(void) {
    printf("  Testing asn_16bit_documentation_64499_irrFallback_doNotInherit+authoritative_2440...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrFallback_doNotInherit+authoritative_2440.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrFallback_doNotInherit_2439(void) {
    printf("  Testing asn_16bit_documentation_64499_irrFallback_doNotInherit_2439...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrFallback_doNotInherit_2439.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrFallback_none_2437(void) {
    printf("  Testing asn_16bit_documentation_64499_irrFallback_none_2437...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrFallback_none_2437.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrLock_authoritative_2442(void) {
    printf("  Testing asn_16bit_documentation_64499_irrLock_authoritative_2442...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrLock_authoritative_2442.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrLock_doNotInherit+authoritative_2444(void) {
    printf("  Testing asn_16bit_documentation_64499_irrLock_doNotInherit+authoritative_2444...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrLock_doNotInherit+authoritative_2444.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrLock_doNotInherit_2443(void) {
    printf("  Testing asn_16bit_documentation_64499_irrLock_doNotInherit_2443...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrLock_doNotInherit_2443.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_irrLock_none_2441(void) {
    printf("  Testing asn_16bit_documentation_64499_irrLock_none_2441...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_irrLock_none_2441.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_rasaOnly_authoritative_2446(void) {
    printf("  Testing asn_16bit_documentation_64499_rasaOnly_authoritative_2446...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_rasaOnly_authoritative_2446.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_rasaOnly_doNotInherit+authoritative_2448(void) {
    printf("  Testing asn_16bit_documentation_64499_rasaOnly_doNotInherit+authoritative_2448...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_rasaOnly_doNotInherit+authoritative_2448.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_rasaOnly_doNotInherit_2447(void) {
    printf("  Testing asn_16bit_documentation_64499_rasaOnly_doNotInherit_2447...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_rasaOnly_doNotInherit_2447.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64499_rasaOnly_none_2445(void) {
    printf("  Testing asn_16bit_documentation_64499_rasaOnly_none_2445...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64499_rasaOnly_none_2445.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrFallback_authoritative_2450(void) {
    printf("  Testing asn_16bit_documentation_64500_irrFallback_authoritative_2450...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrFallback_authoritative_2450.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrFallback_doNotInherit+authoritative_2452(void) {
    printf("  Testing asn_16bit_documentation_64500_irrFallback_doNotInherit+authoritative_2452...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrFallback_doNotInherit+authoritative_2452.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrFallback_doNotInherit_2451(void) {
    printf("  Testing asn_16bit_documentation_64500_irrFallback_doNotInherit_2451...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrFallback_doNotInherit_2451.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrFallback_none_2449(void) {
    printf("  Testing asn_16bit_documentation_64500_irrFallback_none_2449...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrFallback_none_2449.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrLock_authoritative_2454(void) {
    printf("  Testing asn_16bit_documentation_64500_irrLock_authoritative_2454...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrLock_authoritative_2454.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrLock_doNotInherit+authoritative_2456(void) {
    printf("  Testing asn_16bit_documentation_64500_irrLock_doNotInherit+authoritative_2456...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrLock_doNotInherit+authoritative_2456.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrLock_doNotInherit_2455(void) {
    printf("  Testing asn_16bit_documentation_64500_irrLock_doNotInherit_2455...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrLock_doNotInherit_2455.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_irrLock_none_2453(void) {
    printf("  Testing asn_16bit_documentation_64500_irrLock_none_2453...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_irrLock_none_2453.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_rasaOnly_authoritative_2458(void) {
    printf("  Testing asn_16bit_documentation_64500_rasaOnly_authoritative_2458...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_rasaOnly_authoritative_2458.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_rasaOnly_doNotInherit+authoritative_2460(void) {
    printf("  Testing asn_16bit_documentation_64500_rasaOnly_doNotInherit+authoritative_2460...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_rasaOnly_doNotInherit+authoritative_2460.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_rasaOnly_doNotInherit_2459(void) {
    printf("  Testing asn_16bit_documentation_64500_rasaOnly_doNotInherit_2459...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_rasaOnly_doNotInherit_2459.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64500_rasaOnly_none_2457(void) {
    printf("  Testing asn_16bit_documentation_64500_rasaOnly_none_2457...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64500_rasaOnly_none_2457.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrFallback_authoritative_2462(void) {
    printf("  Testing asn_16bit_documentation_64501_irrFallback_authoritative_2462...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrFallback_authoritative_2462.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrFallback_doNotInherit+authoritative_2464(void) {
    printf("  Testing asn_16bit_documentation_64501_irrFallback_doNotInherit+authoritative_2464...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrFallback_doNotInherit+authoritative_2464.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrFallback_doNotInherit_2463(void) {
    printf("  Testing asn_16bit_documentation_64501_irrFallback_doNotInherit_2463...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrFallback_doNotInherit_2463.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrFallback_none_2461(void) {
    printf("  Testing asn_16bit_documentation_64501_irrFallback_none_2461...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrFallback_none_2461.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrLock_authoritative_2466(void) {
    printf("  Testing asn_16bit_documentation_64501_irrLock_authoritative_2466...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrLock_authoritative_2466.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrLock_doNotInherit+authoritative_2468(void) {
    printf("  Testing asn_16bit_documentation_64501_irrLock_doNotInherit+authoritative_2468...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrLock_doNotInherit+authoritative_2468.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrLock_doNotInherit_2467(void) {
    printf("  Testing asn_16bit_documentation_64501_irrLock_doNotInherit_2467...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrLock_doNotInherit_2467.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_irrLock_none_2465(void) {
    printf("  Testing asn_16bit_documentation_64501_irrLock_none_2465...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_irrLock_none_2465.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_rasaOnly_authoritative_2470(void) {
    printf("  Testing asn_16bit_documentation_64501_rasaOnly_authoritative_2470...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_rasaOnly_authoritative_2470.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_rasaOnly_doNotInherit+authoritative_2472(void) {
    printf("  Testing asn_16bit_documentation_64501_rasaOnly_doNotInherit+authoritative_2472...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_rasaOnly_doNotInherit+authoritative_2472.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_rasaOnly_doNotInherit_2471(void) {
    printf("  Testing asn_16bit_documentation_64501_rasaOnly_doNotInherit_2471...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_rasaOnly_doNotInherit_2471.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64501_rasaOnly_none_2469(void) {
    printf("  Testing asn_16bit_documentation_64501_rasaOnly_none_2469...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64501_rasaOnly_none_2469.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrFallback_authoritative_2474(void) {
    printf("  Testing asn_16bit_documentation_64502_irrFallback_authoritative_2474...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrFallback_authoritative_2474.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrFallback_doNotInherit+authoritative_2476(void) {
    printf("  Testing asn_16bit_documentation_64502_irrFallback_doNotInherit+authoritative_2476...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrFallback_doNotInherit+authoritative_2476.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrFallback_doNotInherit_2475(void) {
    printf("  Testing asn_16bit_documentation_64502_irrFallback_doNotInherit_2475...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrFallback_doNotInherit_2475.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrFallback_none_2473(void) {
    printf("  Testing asn_16bit_documentation_64502_irrFallback_none_2473...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrFallback_none_2473.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrLock_authoritative_2478(void) {
    printf("  Testing asn_16bit_documentation_64502_irrLock_authoritative_2478...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrLock_authoritative_2478.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrLock_doNotInherit+authoritative_2480(void) {
    printf("  Testing asn_16bit_documentation_64502_irrLock_doNotInherit+authoritative_2480...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrLock_doNotInherit+authoritative_2480.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrLock_doNotInherit_2479(void) {
    printf("  Testing asn_16bit_documentation_64502_irrLock_doNotInherit_2479...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrLock_doNotInherit_2479.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_irrLock_none_2477(void) {
    printf("  Testing asn_16bit_documentation_64502_irrLock_none_2477...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_irrLock_none_2477.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_rasaOnly_authoritative_2482(void) {
    printf("  Testing asn_16bit_documentation_64502_rasaOnly_authoritative_2482...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_rasaOnly_authoritative_2482.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_rasaOnly_doNotInherit+authoritative_2484(void) {
    printf("  Testing asn_16bit_documentation_64502_rasaOnly_doNotInherit+authoritative_2484...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_rasaOnly_doNotInherit+authoritative_2484.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_rasaOnly_doNotInherit_2483(void) {
    printf("  Testing asn_16bit_documentation_64502_rasaOnly_doNotInherit_2483...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_rasaOnly_doNotInherit_2483.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64502_rasaOnly_none_2481(void) {
    printf("  Testing asn_16bit_documentation_64502_rasaOnly_none_2481...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64502_rasaOnly_none_2481.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrFallback_authoritative_2486(void) {
    printf("  Testing asn_16bit_documentation_64503_irrFallback_authoritative_2486...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrFallback_authoritative_2486.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrFallback_doNotInherit+authoritative_2488(void) {
    printf("  Testing asn_16bit_documentation_64503_irrFallback_doNotInherit+authoritative_2488...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrFallback_doNotInherit+authoritative_2488.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrFallback_doNotInherit_2487(void) {
    printf("  Testing asn_16bit_documentation_64503_irrFallback_doNotInherit_2487...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrFallback_doNotInherit_2487.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrFallback_none_2485(void) {
    printf("  Testing asn_16bit_documentation_64503_irrFallback_none_2485...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrFallback_none_2485.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrLock_authoritative_2490(void) {
    printf("  Testing asn_16bit_documentation_64503_irrLock_authoritative_2490...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrLock_authoritative_2490.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrLock_doNotInherit+authoritative_2492(void) {
    printf("  Testing asn_16bit_documentation_64503_irrLock_doNotInherit+authoritative_2492...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrLock_doNotInherit+authoritative_2492.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrLock_doNotInherit_2491(void) {
    printf("  Testing asn_16bit_documentation_64503_irrLock_doNotInherit_2491...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrLock_doNotInherit_2491.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_irrLock_none_2489(void) {
    printf("  Testing asn_16bit_documentation_64503_irrLock_none_2489...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_irrLock_none_2489.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_rasaOnly_authoritative_2494(void) {
    printf("  Testing asn_16bit_documentation_64503_rasaOnly_authoritative_2494...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_rasaOnly_authoritative_2494.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_rasaOnly_doNotInherit+authoritative_2496(void) {
    printf("  Testing asn_16bit_documentation_64503_rasaOnly_doNotInherit+authoritative_2496...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_rasaOnly_doNotInherit+authoritative_2496.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_rasaOnly_doNotInherit_2495(void) {
    printf("  Testing asn_16bit_documentation_64503_rasaOnly_doNotInherit_2495...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_rasaOnly_doNotInherit_2495.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64503_rasaOnly_none_2493(void) {
    printf("  Testing asn_16bit_documentation_64503_rasaOnly_none_2493...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64503_rasaOnly_none_2493.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64504_irrFallback_authoritative_2498(void) {
    printf("  Testing asn_16bit_documentation_64504_irrFallback_authoritative_2498...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64504_irrFallback_authoritative_2498.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64504_irrFallback_doNotInherit+authoritative_2500(void) {
    printf("  Testing asn_16bit_documentation_64504_irrFallback_doNotInherit+authoritative_2500...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64504_irrFallback_doNotInherit+authoritative_2500.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64504_irrFallback_doNotInherit_2499(void) {
    printf("  Testing asn_16bit_documentation_64504_irrFallback_doNotInherit_2499...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64504_irrFallback_doNotInherit_2499.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}



static int test_asn_16bit_documentation_64504_irrFallback_none_2497(void) {
    printf("  Testing asn_16bit_documentation_64504_irrFallback_none_2497...\n");
    
    /* Load and parse RASA-SET config */
    rasa_set_config_t *set_config = NULL;
    int result = rasa_set_load_from_json("../fixtures/asn_16bit_documentation_64504_irrFallback_none_2497.json", &set_config);
    
        /* Validate ASN range */
    if (set_config) {
        TEST_ASSERT(set_config->containing_as > 0, "AS should be positive");
        TEST_ASSERT(set_config->containing_as <= 4294967295, "AS should fit in 32 bits");
    }
    
    if (set_config) {
        rasa_set_free(set_config);
    }
    
    return 1 ? 0 : 1;
}


int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("RASA Generated Test Suite\n");
    printf("========================================\n\n");

    test_asn_16bit_documentation_64496_irrFallback_authoritative_2402();
    test_asn_16bit_documentation_64496_irrFallback_doNotInherit+authoritative_2404();
    test_asn_16bit_documentation_64496_irrFallback_doNotInherit_2403();
    test_asn_16bit_documentation_64496_irrFallback_none_2401();
    test_asn_16bit_documentation_64496_irrLock_authoritative_2406();
    test_asn_16bit_documentation_64496_irrLock_doNotInherit+authoritative_2408();
    test_asn_16bit_documentation_64496_irrLock_doNotInherit_2407();
    test_asn_16bit_documentation_64496_irrLock_none_2405();
    test_asn_16bit_documentation_64496_rasaOnly_authoritative_2410();
    test_asn_16bit_documentation_64496_rasaOnly_doNotInherit+authoritative_2412();
    test_asn_16bit_documentation_64496_rasaOnly_doNotInherit_2411();
    test_asn_16bit_documentation_64496_rasaOnly_none_2409();
    test_asn_16bit_documentation_64497_irrFallback_authoritative_2414();
    test_asn_16bit_documentation_64497_irrFallback_doNotInherit+authoritative_2416();
    test_asn_16bit_documentation_64497_irrFallback_doNotInherit_2415();
    test_asn_16bit_documentation_64497_irrFallback_none_2413();
    test_asn_16bit_documentation_64497_irrLock_authoritative_2418();
    test_asn_16bit_documentation_64497_irrLock_doNotInherit+authoritative_2420();
    test_asn_16bit_documentation_64497_irrLock_doNotInherit_2419();
    test_asn_16bit_documentation_64497_irrLock_none_2417();
    test_asn_16bit_documentation_64497_rasaOnly_authoritative_2422();
    test_asn_16bit_documentation_64497_rasaOnly_doNotInherit+authoritative_2424();
    test_asn_16bit_documentation_64497_rasaOnly_doNotInherit_2423();
    test_asn_16bit_documentation_64497_rasaOnly_none_2421();
    test_asn_16bit_documentation_64498_irrFallback_authoritative_2426();
    test_asn_16bit_documentation_64498_irrFallback_doNotInherit+authoritative_2428();
    test_asn_16bit_documentation_64498_irrFallback_doNotInherit_2427();
    test_asn_16bit_documentation_64498_irrFallback_none_2425();
    test_asn_16bit_documentation_64498_irrLock_authoritative_2430();
    test_asn_16bit_documentation_64498_irrLock_doNotInherit+authoritative_2432();
    test_asn_16bit_documentation_64498_irrLock_doNotInherit_2431();
    test_asn_16bit_documentation_64498_irrLock_none_2429();
    test_asn_16bit_documentation_64498_rasaOnly_authoritative_2434();
    test_asn_16bit_documentation_64498_rasaOnly_doNotInherit+authoritative_2436();
    test_asn_16bit_documentation_64498_rasaOnly_doNotInherit_2435();
    test_asn_16bit_documentation_64498_rasaOnly_none_2433();
    test_asn_16bit_documentation_64499_irrFallback_authoritative_2438();
    test_asn_16bit_documentation_64499_irrFallback_doNotInherit+authoritative_2440();
    test_asn_16bit_documentation_64499_irrFallback_doNotInherit_2439();
    test_asn_16bit_documentation_64499_irrFallback_none_2437();
    test_asn_16bit_documentation_64499_irrLock_authoritative_2442();
    test_asn_16bit_documentation_64499_irrLock_doNotInherit+authoritative_2444();
    test_asn_16bit_documentation_64499_irrLock_doNotInherit_2443();
    test_asn_16bit_documentation_64499_irrLock_none_2441();
    test_asn_16bit_documentation_64499_rasaOnly_authoritative_2446();
    test_asn_16bit_documentation_64499_rasaOnly_doNotInherit+authoritative_2448();
    test_asn_16bit_documentation_64499_rasaOnly_doNotInherit_2447();
    test_asn_16bit_documentation_64499_rasaOnly_none_2445();
    test_asn_16bit_documentation_64500_irrFallback_authoritative_2450();
    test_asn_16bit_documentation_64500_irrFallback_doNotInherit+authoritative_2452();
    test_asn_16bit_documentation_64500_irrFallback_doNotInherit_2451();
    test_asn_16bit_documentation_64500_irrFallback_none_2449();
    test_asn_16bit_documentation_64500_irrLock_authoritative_2454();
    test_asn_16bit_documentation_64500_irrLock_doNotInherit+authoritative_2456();
    test_asn_16bit_documentation_64500_irrLock_doNotInherit_2455();
    test_asn_16bit_documentation_64500_irrLock_none_2453();
    test_asn_16bit_documentation_64500_rasaOnly_authoritative_2458();
    test_asn_16bit_documentation_64500_rasaOnly_doNotInherit+authoritative_2460();
    test_asn_16bit_documentation_64500_rasaOnly_doNotInherit_2459();
    test_asn_16bit_documentation_64500_rasaOnly_none_2457();
    test_asn_16bit_documentation_64501_irrFallback_authoritative_2462();
    test_asn_16bit_documentation_64501_irrFallback_doNotInherit+authoritative_2464();
    test_asn_16bit_documentation_64501_irrFallback_doNotInherit_2463();
    test_asn_16bit_documentation_64501_irrFallback_none_2461();
    test_asn_16bit_documentation_64501_irrLock_authoritative_2466();
    test_asn_16bit_documentation_64501_irrLock_doNotInherit+authoritative_2468();
    test_asn_16bit_documentation_64501_irrLock_doNotInherit_2467();
    test_asn_16bit_documentation_64501_irrLock_none_2465();
    test_asn_16bit_documentation_64501_rasaOnly_authoritative_2470();
    test_asn_16bit_documentation_64501_rasaOnly_doNotInherit+authoritative_2472();
    test_asn_16bit_documentation_64501_rasaOnly_doNotInherit_2471();
    test_asn_16bit_documentation_64501_rasaOnly_none_2469();
    test_asn_16bit_documentation_64502_irrFallback_authoritative_2474();
    test_asn_16bit_documentation_64502_irrFallback_doNotInherit+authoritative_2476();
    test_asn_16bit_documentation_64502_irrFallback_doNotInherit_2475();
    test_asn_16bit_documentation_64502_irrFallback_none_2473();
    test_asn_16bit_documentation_64502_irrLock_authoritative_2478();
    test_asn_16bit_documentation_64502_irrLock_doNotInherit+authoritative_2480();
    test_asn_16bit_documentation_64502_irrLock_doNotInherit_2479();
    test_asn_16bit_documentation_64502_irrLock_none_2477();
    test_asn_16bit_documentation_64502_rasaOnly_authoritative_2482();
    test_asn_16bit_documentation_64502_rasaOnly_doNotInherit+authoritative_2484();
    test_asn_16bit_documentation_64502_rasaOnly_doNotInherit_2483();
    test_asn_16bit_documentation_64502_rasaOnly_none_2481();
    test_asn_16bit_documentation_64503_irrFallback_authoritative_2486();
    test_asn_16bit_documentation_64503_irrFallback_doNotInherit+authoritative_2488();
    test_asn_16bit_documentation_64503_irrFallback_doNotInherit_2487();
    test_asn_16bit_documentation_64503_irrFallback_none_2485();
    test_asn_16bit_documentation_64503_irrLock_authoritative_2490();
    test_asn_16bit_documentation_64503_irrLock_doNotInherit+authoritative_2492();
    test_asn_16bit_documentation_64503_irrLock_doNotInherit_2491();
    test_asn_16bit_documentation_64503_irrLock_none_2489();
    test_asn_16bit_documentation_64503_rasaOnly_authoritative_2494();
    test_asn_16bit_documentation_64503_rasaOnly_doNotInherit+authoritative_2496();
    test_asn_16bit_documentation_64503_rasaOnly_doNotInherit_2495();
    test_asn_16bit_documentation_64503_rasaOnly_none_2493();
    test_asn_16bit_documentation_64504_irrFallback_authoritative_2498();
    test_asn_16bit_documentation_64504_irrFallback_doNotInherit+authoritative_2500();
    test_asn_16bit_documentation_64504_irrFallback_doNotInherit_2499();
    test_asn_16bit_documentation_64504_irrFallback_none_2497();

    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_passed + tests_failed);
    printf("========================================\n");

    return tests_failed > 0 ? 1 : 0;
}