/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * RASA (RPKI AS-SET Authorization) support for bgpq4
 */

#ifndef RASA_H
#define RASA_H

#include <stdint.h>

struct rasa_config {
	int enabled;
	char *source_file;
};

struct rasa_auth {
	uint32_t asn;
	int authorized;
	char *reason;
};

int rasa_load_config(struct rasa_config *cfg, const char *filename);
int rasa_check_auth(uint32_t asn, const char *asset, struct rasa_auth *result);
void rasa_free_config(struct rasa_config *cfg);

#endif
