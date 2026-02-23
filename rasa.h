/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 *
 * RASA (RPKI AS-SET Authorization) support for bgpq4
 * Supports both RASA-AUTH (AS authorizes AS-SET membership)
 * and RASA-SET (AS-SET declares member ASes)
 */

#ifndef RASA_H
#define RASA_H

#include <stdint.h>

/* RASA-AUTH configuration */
struct rasa_config {
	int enabled;
	char *source_file;
};

/* RASA-SET configuration */
struct rasa_set_config {
	int enabled;
	char *source_file;
};

/* RASA authorization result (from AS perspective) */
struct rasa_auth {
	uint32_t asn;
	int authorized;
	char *reason;
};

/* RASA-SET membership result (from AS-SET perspective) */
struct rasa_set_membership {
	const char *asset;
	uint32_t asn;
	int is_member;
	char *reason;
};

/* Function prototypes for RASA-AUTH */
int rasa_load_config(struct rasa_config *cfg, const char *filename);
int rasa_check_auth(uint32_t asn, const char *asset, struct rasa_auth *result);
void rasa_free_config(struct rasa_config *cfg);

/* Function prototypes for RASA-SET */
int rasa_set_load_config(struct rasa_set_config *cfg, const char *filename);
int rasa_check_set_membership(const char *asset, uint32_t asn, 
    struct rasa_set_membership *result);
void rasa_set_free_config(struct rasa_set_config *cfg);

/* Bidirectional verification */
int rasa_verify_bidirectional(const char *asset, uint32_t asn,
    struct rasa_auth *auth_result, struct rasa_set_membership *set_result);

#endif
