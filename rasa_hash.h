#ifndef RASA_HASH_H
#define RASA_HASH_H

#include <stdint.h>
#include <stddef.h>

#define RASA_FALLBACK_MODE_IRR_FALLBACK 0
#define RASA_FALLBACK_MODE_IRR_LOCK     1
#define RASA_FALLBACK_MODE_RASA_ONLY    2

struct rasa_set_entry {
    char *as_set_name;
    int fallback_mode;
    char *irr_source;
    uint32_t *members;
    size_t num_members;
    char **nested_sets;
    size_t num_nested;
    uint32_t containing_as;
};

struct rasa_hash_bucket {
    struct rasa_set_entry *entry;
    unsigned long hash;
    struct rasa_hash_bucket *next;
};

struct rasa_hash_table {
    struct rasa_hash_bucket **buckets;
    size_t capacity;
    size_t count;
};

struct rasa_hash_table *rasa_hash_table_create(size_t initial_capacity);
void rasa_hash_table_free(struct rasa_hash_table *table);
int rasa_hash_table_insert(struct rasa_hash_table *table, struct rasa_set_entry *entry);
struct rasa_set_entry *rasa_hash_table_lookup(struct rasa_hash_table *table, const char *as_set_name);
int rasa_hash_table_remove(struct rasa_hash_table *table, const char *as_set_name);
size_t rasa_hash_table_count(struct rasa_hash_table *table);

#endif
