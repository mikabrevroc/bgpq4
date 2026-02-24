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
 */

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
