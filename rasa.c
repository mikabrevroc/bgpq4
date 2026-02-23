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

	fprintf(stderr, "RASA: Attempting to load %s\n", filename);
	rasa_data = json_load_file(filename, 0, &error);
	if (!rasa_data) {
		fprintf(stderr, "RASA: failed to load %s: %s\n", filename, error.text);
		return -1;
	}

	fprintf(stderr, "RASA: Successfully loaded JSON\n");
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
		fprintf(stderr, "RASA: Check auth called with null result\n");
		return -1;
	}

	fprintf(stderr, "RASA: Checking authorization for AS%u in %s\n", asn, asset ? asset : "(null)");

	result->asn = asn;
	result->authorized = 1;
	result->reason = NULL;

	if (!rasa_data) {
		fprintf(stderr, "RASA: No RASA data loaded, allowing by default\n");
		result->reason = "no RASA config (default allow)";
		return 0;
	}

	rasas = json_object_get(rasa_data, "rasas");
	if (!rasas || !json_is_array(rasas)) {
		fprintf(stderr, "RASA: No 'rasas' array found, allowing all\n");
		result->reason = "no RASA data (default allow)";
		return 0;
	}

	fprintf(stderr, "RASA: Found %zu RASA entries\n", json_array_size(rasas));

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

		fprintf(stderr, "RASA: Found RASA entry for AS%u\n", asn);

		authorized_in = json_object_get(rasa_obj, "authorized_in");
		if (!authorized_in || !json_is_array(authorized_in)) {
			fprintf(stderr, "RASA: No authorized_in array for AS%u, denying\n", asn);
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
				fprintf(stderr, "RASA: AS%u authorized in %s, checking against %s\n", 
				        asn, auth_asset, asset ? asset : "(null)");
				
				if (asset && strcmp(auth_asset, asset) == 0) {
					fprintf(stderr, "RASA: AS%u AUTHORIZED in %s\n", asn, asset);
					result->authorized = 1;
					result->reason = "authorized";
					return 0;
				}
			}
		}

		fprintf(stderr, "RASA: AS%u NOT authorized in %s\n", asn, asset ? asset : "(null)");
		result->authorized = 0;
		result->reason = "not in authorized_in";
		return 0;
	}

	fprintf(stderr, "RASA: AS%u not found in RASA data, allowing by default\n", asn);
	result->reason = "no RASA-AUTH for this ASN (default allow)";
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
