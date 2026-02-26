/*
 * Copyright (c) 2025 Mikael Abrahamsson <mikael.abrahamsson@fitaliv.se>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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

/* Fallback mode values */
#define RASA_FALLBACK_MODE_IRR_FALLBACK	0	/* Merge RASA with IRR (default) */
#define RASA_FALLBACK_MODE_IRR_LOCK	1	/* Lock to specific IRR database */
#define RASA_FALLBACK_MODE_RASA_ONLY	2	/* Use only RASA data */

/* RASA-SET configuration */
struct rasa_set_config {
	int enabled;
	char *source_file;
	int fallback_mode;	/* Fallback mode (0=irrFallback, 1=irrLock, 2=rasaOnly) */
	char *irr_source;	/* IRR source for irrLock mode */
	char **nested_sets;	/* Nested AS-SET names */
	size_t num_nested;	/* Number of nested sets */
	uint32_t containing_as;	/* Owning AS number */
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

/* New hash table based API for per-AS-SET lookup */
struct rasa_set_entry;
int rasa_load_sets_from_json(const char *filename);
struct rasa_set_entry *rasa_lookup_set(const char *as_set_name);
void rasa_cleanup_sets(void);
int rasa_set_count(void);

#endif
