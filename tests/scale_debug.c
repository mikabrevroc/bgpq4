#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../rasa.h"
#include "../rasa_hash.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <rasa_set.json>\n", argv[0]);
        return 1;
    }

    printf("Loading: %s\n", argv[1]);
    
    int result = rasa_load_sets_from_json(argv[1]);
    printf("Load result: %d\n", result);
    
    printf("Looking up AS-SCALE-TEST...\n");
    struct rasa_set_entry *entry = rasa_lookup_set("AS-SCALE-TEST");
    
    if (entry) {
        printf("Found entry:\n");
        printf("  as_set_name: %s\n", entry->as_set_name ? entry->as_set_name : "(null)");
        printf("  fallback_mode: %d\n", entry->fallback_mode);
        printf("  num_members: %zu\n", entry->num_members);
    } else {
        printf("Entry NOT found\n");
    }
    
    return 0;
}
