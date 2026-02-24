/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * RASA (RPKI AS-SET Authorization) implementation
 * Minimal implementation - reads JSON output from rpki-client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "rasa.h"
#include "rasa_hash.h"

static json_t *rasa_data = NULL;

int
rasa_load_config(struct rasa_config *cfg, const char *filename)
{
	json_error_t error;

	if (!filename)
		return -1;

	rasa_data = json_load_file(filename, 0, &error);
	if (!rasa_data) {
		return -1;
	}
	cfg->enabled = 1;
	cfg->source_file = strdup(filename);
	return 0;
}

int
rasa_check_auth(uint32_t asn, const char *asset, struct rasa_auth *result)
{
	json_t *rasas, *rasa_entry, *rasa_obj, *authorized_in;
	size_t i;

	if (!result) {
		return -1;
	}

	result->asn = asn;
	result->authorized = 0;
	result->reason = NULL;

	if (!rasa_data) {
		result->authorized = 1;
		result->reason = "no RASA config (default allow)";
		return 0;
	}

	rasas = json_object_get(rasa_data, "rasas");
	if (!rasas || !json_is_array(rasas)) {
		result->authorized = 1;
		result->reason = "no RASA data (default allow)";
		return 0;
	}

	if (json_array_size(rasas) == 0) {
		result->authorized = 1;
		result->reason = "empty RASA config (default allow)";
		return 0;
	}

	for (i = 0; i < json_array_size(rasas); i++) {
		rasa_entry = json_array_get(rasas, i);
		if (!rasa_entry)
			continue;

		rasa_obj = json_object_get(rasa_entry, "rasa");
		if (!rasa_obj)
			continue;

		json_t *asn_obj = json_object_get(rasa_obj, "authorized_as");
		if (!json_is_integer(asn_obj) || (uint32_t)json_integer_value(asn_obj) != asn)
			continue;

		authorized_in = json_object_get(rasa_obj, "authorized_in");
		if (!authorized_in || !json_is_array(authorized_in)) {
			result->authorized = 0;
			result->reason = "no authorized_in";
			return 0;
		}

		size_t j;
		for (j = 0; j < json_array_size(authorized_in); j++) {
			json_t *entry = json_array_get(authorized_in, j);
			if (!entry)
				continue;

			json_t *entry_obj = json_object_get(entry, "entry");
			if (!entry_obj)
				continue;

			json_t *asset_name = json_object_get(entry_obj, "asset");
			
			if (json_is_string(asset_name)) {
				const char *auth_asset = json_string_value(asset_name);
				
				if (asset && strcmp(auth_asset, asset) == 0) {
					result->authorized = 1;
					result->reason = "authorized";
					return 0;
				}
			}
		}

		result->authorized = 0;
		result->reason = "not in authorized_in";
		return 0;
	}

	result->authorized = 0;
	result->reason = "no RASA-AUTH for this ASN";
	return 0;
}

void
rasa_free_config(struct rasa_config *cfg)
{
	if (cfg->source_file) {
		free(cfg->source_file);
		cfg->source_file = NULL;
	}
	if (rasa_data) {
		json_decref(rasa_data);
		rasa_data = NULL;
	}
	cfg->enabled = 0;
}

/*
 * RASA-SET implementation
 * For AS-SET owners to declare member ASes
 */

static json_t *rasa_set_data = NULL;

int
rasa_set_load_config(struct rasa_set_config *cfg, const char *filename)
{
	json_error_t error;
	json_t *rasasets, *set_entry, *set_obj;
	size_t i;

	if (!filename)
		return -1;

	rasa_set_data = json_load_file(filename, 0, &error);
	if (!rasa_set_data) {
		return -1;
	}

	/* Parse first RASA-SET to extract config fields */
	rasasets = json_object_get(rasa_set_data, "rasa_sets");
	if (rasasets && json_is_array(rasasets) && json_array_size(rasasets) > 0) {
		set_entry = json_array_get(rasasets, 0);
		if (set_entry) {
			set_obj = json_object_get(set_entry, "rasa_set");
			if (set_obj) {
				/* Parse fallback_mode */
				json_t *fallback_mode = json_object_get(set_obj, "fallback_mode");
				if (fallback_mode && json_is_string(fallback_mode)) {
					const char *mode = json_string_value(fallback_mode);
					if (strcmp(mode, "irrLock") == 0)
						cfg->fallback_mode = RASA_FALLBACK_MODE_IRR_LOCK;
					else if (strcmp(mode, "rasaOnly") == 0)
						cfg->fallback_mode = RASA_FALLBACK_MODE_RASA_ONLY;
					else
						cfg->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
				} else {
					cfg->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
				}

				/* Parse irr_source */
				json_t *irr_source = json_object_get(set_obj, "irr_source");
				if (irr_source && json_is_string(irr_source)) {
					cfg->irr_source = strdup(json_string_value(irr_source));
				}

				/* Parse containing_as */
				json_t *containing_as = json_object_get(set_obj, "containing_as");
				if (containing_as && json_is_integer(containing_as)) {
					cfg->containing_as = (uint32_t)json_integer_value(containing_as);
				}

				/* Parse nested_sets */
				json_t *nested_sets = json_object_get(set_obj, "nested_sets");
				if (nested_sets && json_is_array(nested_sets)) {
					cfg->num_nested = json_array_size(nested_sets);
					if (cfg->num_nested > 0) {
						cfg->nested_sets = calloc(cfg->num_nested, sizeof(char *));
						for (i = 0; i < cfg->num_nested; i++) {
							json_t *nested = json_array_get(nested_sets, i);
							if (nested && json_is_string(nested)) {
								cfg->nested_sets[i] = strdup(json_string_value(nested));
							}
						}
					}
				}
			}
		}
	}

	cfg->enabled = 1;
	cfg->source_file = strdup(filename);
	return 0;
}

int
rasa_check_set_membership(const char *asset, uint32_t asn,
    struct rasa_set_membership *result)
{
	json_t *rasasets, *set_entry, *set_obj, *members;
	size_t i;

	if (!result) {
		return -1;
	}

	result->asset = asset;
	result->asn = asn;
	result->is_member = 1;
	result->reason = NULL;

	if (!rasa_set_data) {
		result->reason = "no RASA-SET config (default allow)";
		return 0;
	}

	rasasets = json_object_get(rasa_set_data, "rasa_sets");
	if (!rasasets || !json_is_array(rasasets)) {
		result->reason = "no RASA-SET data (default allow)";
		return 0;
	}

	for (i = 0; i < json_array_size(rasasets); i++) {
		set_entry = json_array_get(rasasets, i);
		if (!set_entry)
			continue;

		set_obj = json_object_get(set_entry, "rasa_set");
		if (!set_obj)
			continue;

		json_t *name_obj = json_object_get(set_obj, "as_set_name");
		if (!json_is_string(name_obj))
			continue;

		const char *set_name = json_string_value(name_obj);
		if (!asset || strcmp(set_name, asset) != 0)
			continue;

		members = json_object_get(set_obj, "members");
		if (!members || !json_is_array(members)) {
			result->is_member = 0;
			result->reason = "no members declared";
			return 0;
		}

		size_t j;
		for (j = 0; j < json_array_size(members); j++) {
			json_t *member = json_array_get(members, j);
			if (!member)
				continue;

			if (json_is_integer(member)) {
				uint32_t member_asn = (uint32_t)json_integer_value(member);
				if (member_asn == asn) {
					result->is_member = 1;
					result->reason = "member";
					return 0;
				}
			}
		}

		result->is_member = 0;
		result->reason = "not in members list";
		return 0;
	}

	result->reason = "no RASA-SET for this AS-SET (default allow)";
	return 0;
}

void
rasa_set_free_config(struct rasa_set_config *cfg)
{
	size_t i;

	if (cfg->source_file) {
		free(cfg->source_file);
		cfg->source_file = NULL;
	}
	if (cfg->irr_source) {
		free(cfg->irr_source);
		cfg->irr_source = NULL;
	}
	if (cfg->nested_sets) {
		for (i = 0; i < cfg->num_nested; i++) {
			if (cfg->nested_sets[i])
				free(cfg->nested_sets[i]);
		}
		free(cfg->nested_sets);
		cfg->nested_sets = NULL;
		cfg->num_nested = 0;
	}
	if (rasa_set_data) {
		json_decref(rasa_set_data);
		rasa_set_data = NULL;
	}
	cfg->enabled = 0;
	cfg->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
}

/*
 * Bidirectional verification
 * Combines RASA-AUTH and RASA-SET checks
 * An AS is authorized if either:
 *   1. RASA-AUTH: The AS authorizes being in the AS-SET, OR
 *   2. RASA-SET: The AS-SET declares the AS as a member
 * Both must be checked for complete verification
 */
int
rasa_verify_bidirectional(const char *asset, uint32_t asn,
    struct rasa_auth *auth_result, struct rasa_set_membership *set_result)
{
	int auth_rc, set_rc;

	auth_rc = rasa_check_auth(asn, asset, auth_result);
	set_rc = rasa_check_set_membership(asset, asn, set_result);

	if (auth_rc != 0 || set_rc != 0) {
		return -1;
	}

	return 0;
}

/* New hash table based RASA-SET functions */

static struct rasa_hash_table *rasa_set_table = NULL;

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
        json_t *name_obj, *mode_obj, *source_obj, *members_obj, *nested_obj;
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
        if (json_is_string(mode_obj)) {
            const char *mode_str = json_string_value(mode_obj);
            if (strcmp(mode_str, "irrLock") == 0)
                new_entry->fallback_mode = RASA_FALLBACK_MODE_IRR_LOCK;
            else if (strcmp(mode_str, "rasaOnly") == 0)
                new_entry->fallback_mode = RASA_FALLBACK_MODE_RASA_ONLY;
            else
                new_entry->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
        } else
            new_entry->fallback_mode = RASA_FALLBACK_MODE_IRR_FALLBACK;
        
        source_obj = json_object_get(rasa_set_obj, "irr_source");
        if (json_is_string(source_obj))
            new_entry->irr_source = strdup(json_string_value(source_obj));
        
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
