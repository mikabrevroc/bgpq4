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

	if (!filename)
		return -1;

	rasa_set_data = json_load_file(filename, 0, &error);
	if (!rasa_set_data) {
		return -1;
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
		if (strcmp(set_name, asset) != 0)
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
	if (cfg->source_file) {
		free(cfg->source_file);
		cfg->source_file = NULL;
	}
	if (rasa_set_data) {
		json_decref(rasa_set_data);
		rasa_set_data = NULL;
	}
	cfg->enabled = 0;
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
