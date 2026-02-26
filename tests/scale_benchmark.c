#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        fprintf(stderr, "Usage: %s <rasa_set.json>\n", argv[0]);
        return 1;
    }

    double start = get_time_ms();
    
    if (rasa_load_sets_from_json(argv[1]) != 0) {
        fprintf(stderr, "Failed to load %s\n", argv[1]);
        return 1;
    }
    
    double load_time = get_time_ms() - start;
    
    // Count lookups
    struct rasa_set_entry *entry = rasa_lookup_set("AS-SCALE-TEST");
    int count = (entry && entry->members) ? (int)entry->num_members : 0;
    
    // Benchmark lookups
    start = get_time_ms();
    int iterations = 100000;
    for (int i = 0; i < iterations; i++) {
        rasa_lookup_set("AS-SCALE-TEST");
    }
    double lookup_total = get_time_ms() - start;
    
    printf("Load time: %.2f ms | Members: %d | Lookups: %.2f ms (%.3f ms/op)\n",
           load_time, count, lookup_total, lookup_total / iterations);
    
    return 0;
}
