#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "rasa.h"
#include "rasa_hash.h"

static struct rasa_hash_table *rasa_set_table = NULL;

static int
parse_fallback_mode(const char *mode_str)
{
    if (!mode_str)
        return RASA_FALLBACK_MODE_IRR_FALLBACK;
    
    if (strcmp(mode_str, "irrLock") == 0)
        return RASA_FALLBACK_MODE_IRR_LOCK;
    else if (strcmp(mode_str, "rasaOnly") == 0)
        return RASA_FALLBACK_MODE_RASA_ONLY;
    else
        return RASA_FALLBACK_MODE_IRR_FALLBACK;
}

int
rasa_load_sets_from_json(const char *filename)
{
    json_error_t error;
    json_t *root, *rasa_sets;
    size_t i;
    
    if (!filename)
        return -1;
    
    if (rasa_set_table) {
        rasa_hash_table_free(rasa_set_table);
        rasa_set_table = NULL;
    }
    
    root = json_load_file(filename, 0, &error);
    if (!root)
        return -1;
    
    rasa_set_table = rasa_hash_table_create(64);
    if (!rasa_set_table) {
        json_decref(root);
        return -1;
    }
    
    rasa_sets = json_object_get(root, "rasa_sets");
    if (!json_is_array(rasa_sets)) {
        json_decref(root);
        rasa_hash_table_free(rasa_set_table);
        rasa_set_table = NULL;
        return -1;
    }
    
    for (i = 0; i < json_array_size(rasa_sets); i++) {
        json_t *entry = json_array_get(rasa_sets, i);
        json_t *rasa_set_obj = json_object_get(entry, "rasa_set");
        json_t *name_obj, *mode_obj, *source_obj, *members_obj, *nested_obj, *containing_obj;
        struct rasa_set_entry *new_entry;
        size_t j;
        int ret;
        
        if (!rasa_set_obj)
            continue;
        
        name_obj = json_object_get(rasa_set_obj, "as_set_name");
        if (!json_is_string(name_obj))
            continue;
        
        new_entry = calloc(1, sizeof(*new_entry));
        if (!new_entry)
            continue;
        
        new_entry->as_set_name = strdup(json_string_value(name_obj));
        
        mode_obj = json_object_get(rasa_set_obj, "fallback_mode");
        if (json_is_string(mode_obj))
            new_entry->fallback_mode = parse_fallback_mode(json_string_value(mode_obj));
        else
            new_entry->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
        
        source_obj = json_object_get(rasa_set_obj, "irr_source");
        if (json_is_string(source_obj))
            new_entry->irr_source = strdup(json_string_value(source_obj));
        
        containing_obj = json_object_get(rasa_set_obj, "containing_as");
        if (json_is_integer(containing_obj))
            new_entry->containing_as = (uint32_t)json_integer_value(containing_obj);
        
        members_obj = json_object_get(rasa_set_obj, "members");
        if (json_is_array(members_obj)) {
            new_entry->num_members = json_array_size(members_obj);
            if (new_entry->num_members > 0) {
                new_entry->members = calloc(new_entry->num_members, sizeof(uint32_t));
                for (j = 0; j < new_entry->num_members; j++) {
                    json_t *member = json_array_get(members_obj, j);
                    if (json_is_integer(member))
                        new_entry->members[j] = (uint32_t)json_integer_value(member);
                }
            }
        }
        
        nested_obj = json_object_get(rasa_set_obj, "nested_sets");
        if (json_is_array(nested_obj)) {
            new_entry->num_nested = json_array_size(nested_obj);
            if (new_entry->num_nested > 0) {
                new_entry->nested_sets = calloc(new_entry->num_nested, sizeof(char *));
                for (j = 0; j < new_entry->num_nested; j++) {
                    json_t *nested = json_array_get(nested_obj, j);
                    if (json_is_string(nested))
                        new_entry->nested_sets[j] = strdup(json_string_value(nested));
                }
            }
        }
        
        ret = rasa_hash_table_insert(rasa_set_table, new_entry);
        if (ret == -2) {
            fprintf(stderr, "Error: Multiple RASA-SETs for AS-SET %s\n", new_entry->as_set_name);
            if (new_entry->as_set_name) free(new_entry->as_set_name);
            if (new_entry->irr_source) free(new_entry->irr_source);
            if (new_entry->members) free(new_entry->members);
            if (new_entry->nested_sets) {
                for (j = 0; j < new_entry->num_nested; j++)
                    if (new_entry->nested_sets[j]) free(new_entry->nested_sets[j]);
                free(new_entry->nested_sets);
            }
            free(new_entry);
            json_decref(root);
            rasa_hash_table_free(rasa_set_table);
            rasa_set_table = NULL;
            return -1;
        } else if (ret != 0) {
            if (new_entry->as_set_name) free(new_entry->as_set_name);
            if (new_entry->irr_source) free(new_entry->irr_source);
            if (new_entry->members) free(new_entry->members);
            if (new_entry->nested_sets) {
                for (j = 0; j < new_entry->num_nested; j++)
                    if (new_entry->nested_sets[j]) free(new_entry->nested_sets[j]);
                free(new_entry->nested_sets);
            }
            free(new_entry);
        }
    }
    
    json_decref(root);
    return 0;
}

struct rasa_set_entry *
rasa_lookup_set(const char *as_set_name)
{
    if (!rasa_set_table || !as_set_name)
        return NULL;
    
    return rasa_hash_table_lookup(rasa_set_table, as_set_name);
}

void
rasa_cleanup_sets(void)
{
    if (rasa_set_table) {
        rasa_hash_table_free(rasa_set_table);
        rasa_set_table = NULL;
    }
}

int
rasa_set_count(void)
{
    if (!rasa_set_table)
        return 0;
    
    return (int)rasa_hash_table_count(rasa_set_table);
}
