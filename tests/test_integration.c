#include <stdio.h>
#include <string.h>
#include "../rasa.h"
#include "../rasa_hash.h"

int main() {
    // Load the RASA-SET
    printf("Loading RASA-SET...\n");
    int result = rasa_load_sets_from_json("tests/rasa_he_test.json");
    printf("Load result: %d\n", result);
    
    // Count loaded sets
    int count = rasa_set_count();
    printf("Loaded %d RASA-SETs\n", count);
    
    // Lookup AS-HURRICANE
    struct rasa_set_entry *entry = rasa_lookup_set("AS-HURRICANE");
    if (entry) {
        printf("Found AS-HURRICANE:\n");
        printf("  fallback_mode: %d (1=irrLock)\n", entry->fallback_mode);
        printf("  irr_source: %s\n", entry->irr_source);
        printf("  num_members: %zu\n", entry->num_members);
        return 0;
    } else {
        printf("AS-HURRICANE NOT found\n");
        return 1;
    }
}
