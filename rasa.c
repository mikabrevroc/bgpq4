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
		fprintf(stderr, "RASA: failed to load %s: %s\n", filename, error.text);
		return -1;
	}

	cfg->enabled = 1;
	cfg->source_file = strdup(filename);
	return 0;
}

int
rasa_check_auth(uint32_t asn, const char *asset, struct rasa_auth *result)
{
	json_t *auths, *auth, *authorized_in;
	size_t i;

	if (!rasa_data || !result)
		return -1;

	result->asn = asn;
	result->authorized = 1;
	result->reason = NULL;

	auths = json_object_get(rasa_data, "rasa_auths");
	if (!auths || !json_is_array(auths)) {
		result->reason = "no RASA-AUTH (default allow)";
		return 0;
	}

	for (i = 0; i < json_array_size(auths); i++) {
		auth = json_array_get(auths, i);
		json_t *asn_obj = json_object_get(auth, "authorized_as");
		
		if (!json_is_integer(asn_obj) || json_integer_value(asn_obj) != asn)
			continue;

		authorized_in = json_object_get(auth, "authorized_in");
		if (!authorized_in || !json_is_array(authorized_in))
			continue;

		size_t j;
		for (j = 0; j < json_array_size(authorized_in); j++) {
			json_t *entry = json_array_get(authorized_in, j);
			json_t *asset_name = json_object_get(entry, "as_set_name");
			
			if (json_is_string(asset_name) &&
			    strcmp(json_string_value(asset_name), asset) == 0) {
				result->authorized = 1;
				result->reason = "authorized";
				return 0;
			}
		}

		result->authorized = 0;
		result->reason = "not in authorized_in";
		return 0;
	}

	result->reason = "no RASA-AUTH (default allow)";
	return 0;
}

void
rasa_free_config(struct rasa_config *cfg)
{
	if (cfg->source_file)
		free(cfg->source_file);
	if (rasa_data)
		json_decref(rasa_data);
	cfg->enabled = 0;
}
