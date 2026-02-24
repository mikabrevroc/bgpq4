/* Performance test for RASA loading */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../rasa.h"
#include "../rasa_hash.h"

static double get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <rasa_set.json> [rasa_auth.json]\n", argv[0]);
        return 1;
    }

    const char *set_file = argv[1];
    const char *auth_file = (argc > 2) ? argv[2] : NULL;

    double start, end;

    /* Time RASA-SET loading */
    start = get_time_ms();
    if (rasa_load_sets_from_json(set_file) != 0) {
        fprintf(stderr, "Failed to load RASA-SET from %s\n", set_file);
        return 1;
    }
    end = get_time_ms();
    printf("RASA-SET load time: %.2f ms\n", end - start);

    /* Check if entries loaded */
    struct rasa_set_entry *entry = rasa_lookup_set("AS-SCALE-TEST");
    int count = entry ? 1 : 0;
    printf("RASA-SET entries loaded: %d\n", count);

    /* Time lookups */
    if (count > 0) {
        start = get_time_ms();
        int lookups = 10000;
        for (int i = 0; i < lookups; i++) {
            rasa_lookup_set("AS-SCALE-TEST");
        }
        end = get_time_ms();
        printf("Lookup time (%d iterations): %.2f ms (%.3f ms/op)\n",
               lookups, end - start, (end - start) / lookups);
    }

    /* Time RASA-AUTH loading if provided */
    if (auth_file) {
        struct rasa_config config;
        memset(&config, 0, sizeof(config));

        start = get_time_ms();
        if (rasa_load_config(&config, auth_file) != 0) {
            fprintf(stderr, "Failed to load RASA-AUTH from %s\n", auth_file);
            return 1;
        }
        end = get_time_ms();
        printf("RASA-AUTH load time: %.2f ms\n", end - start);

        rasa_free_config(&config);
    }

    /* Cleanup */
    rasa_cleanup_sets();

    return 0;
}
