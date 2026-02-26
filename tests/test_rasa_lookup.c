#include <stdio.h>
#include <string.h>
#include "../rasa.h"
#include "../rasa_hash.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <json_file> [as_set_name]\n", argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    const char *lookup_name = argv[2] ? argv[2] : "AS-HURRICANE";
    
    printf("Loading: %s\n", filename);
    int result = rasa_load_sets_from_json(filename);
    printf("Load result: %d\n", result);
    
    printf("Looking up %s...\n", lookup_name);
    struct rasa_set_entry *entry = rasa_lookup_set(lookup_name);
    
    if (entry) {
        printf("Found entry:\n");
        printf("  as_set_name: %s\n", entry->as_set_name);
        printf("  fallback_mode: %d\n", entry->fallback_mode);
        printf("  irr_source: %s\n", entry->irr_source ? entry->irr_source : "(null)");
        printf("  num_members: %zu\n", entry->num_members);
    } else {
        printf("Entry NOT found\n");
    }
    
    return entry ? 0 : 1;
}
