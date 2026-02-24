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

#include "rasa_hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned long
hash_string(const char *str)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
	hash = ((hash << 5) + hash) + c;
	return hash;
}

static unsigned long
hash_asset(const char *asset)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *asset++)) {
	if (c >= 'a' && c <= 'z')
	    c = c - 'a' + 'A';
	hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

static int
asset_cmp(const char *a, const char *b)
{
	while (*a && *b) {
	char ca = *a;
	char cb = *b;
	if (ca >= 'a' && ca <= 'z')
	    ca = ca - 'a' + 'A';
	if (cb >= 'a' && cb <= 'z')
	    cb = cb - 'a' + 'A';
	if (ca != cb)
	    return ca - cb;
	a++;
	b++;
	}
	return *a - *b;
}

struct rasa_hash_table *
rasa_hash_table_create(size_t initial_capacity)
{
	struct rasa_hash_table *table;
	
	if (initial_capacity < 16)
	initial_capacity = 16;
	
	table = calloc(1, sizeof(*table));
	if (!table)
	return NULL;
	
	table->buckets = calloc(initial_capacity, sizeof(struct rasa_hash_bucket *));
	if (!table->buckets) {
	free(table);
	return NULL;
	}
	
	table->capacity = initial_capacity;
	table->count = 0;
	
	return table;
}

void
rasa_hash_table_free(struct rasa_hash_table *table)
{
	size_t i;
	
	if (!table)
	return;
	
	for (i = 0; i < table->capacity; i++) {
	struct rasa_hash_bucket *bucket = table->buckets[i];
	while (bucket) {
	    struct rasa_hash_bucket *next = bucket->next;
	    
	    if (bucket->entry) {
	        if (bucket->entry->as_set_name)
	            free(bucket->entry->as_set_name);
	        if (bucket->entry->irr_source)
	            free(bucket->entry->irr_source);
	        if (bucket->entry->members)
	            free(bucket->entry->members);
	        if (bucket->entry->nested_sets) {
	            size_t j;
	            for (j = 0; j < bucket->entry->num_nested; j++) {
	                if (bucket->entry->nested_sets[j])
	                    free(bucket->entry->nested_sets[j]);
	            }
	            free(bucket->entry->nested_sets);
	        }
	        free(bucket->entry);
	    }
	    
	    free(bucket);
	    bucket = next;
	}
	}
	
	free(table->buckets);
	free(table);
}

static int
rasa_hash_table_resize(struct rasa_hash_table *table)
{
	size_t new_capacity = table->capacity * 2;
	struct rasa_hash_bucket **new_buckets;
	size_t i;
	
	new_buckets = calloc(new_capacity, sizeof(struct rasa_hash_bucket *));
	if (!new_buckets)
	return -1;
	
	for (i = 0; i < table->capacity; i++) {
	struct rasa_hash_bucket *bucket = table->buckets[i];
	while (bucket) {
	    struct rasa_hash_bucket *next = bucket->next;
	    unsigned long new_hash = hash_asset(bucket->entry->as_set_name);
	    size_t new_idx = new_hash % new_capacity;
	    
	    bucket->next = new_buckets[new_idx];
	    new_buckets[new_idx] = bucket;
	    
	    bucket = next;
	}
	}
	
	free(table->buckets);
	table->buckets = new_buckets;
	table->capacity = new_capacity;
	
	return 0;
}

int
rasa_hash_table_insert(struct rasa_hash_table *table, struct rasa_set_entry *entry)
{
	unsigned long hash;
	size_t idx;
	struct rasa_hash_bucket *bucket;
	
	if (!table || !entry || !entry->as_set_name)
	return -1;
	
	if (table->count > table->capacity * 3 / 4) {
	if (rasa_hash_table_resize(table) != 0)
	    return -1;
	}
	
	hash = hash_asset(entry->as_set_name);
	idx = hash % table->capacity;
	
	bucket = table->buckets[idx];
	while (bucket) {
	if (asset_cmp(bucket->entry->as_set_name, entry->as_set_name) == 0) {
	    return -2;
	}
	bucket = bucket->next;
	}
	
	bucket = calloc(1, sizeof(*bucket));
	if (!bucket)
	return -1;
	
	bucket->entry = entry;
	bucket->hash = hash;
	
	bucket->next = table->buckets[idx];
	table->buckets[idx] = bucket;
	table->count++;
	
	return 0;
}

struct rasa_set_entry *
rasa_hash_table_lookup(struct rasa_hash_table *table, const char *as_set_name)
{
	unsigned long hash;
	size_t idx;
	struct rasa_hash_bucket *bucket;
	
	if (!table || !as_set_name)
	return NULL;
	
	hash = hash_asset(as_set_name);
	idx = hash % table->capacity;
	
	bucket = table->buckets[idx];
	while (bucket) {
	if (asset_cmp(bucket->entry->as_set_name, as_set_name) == 0)
	    return bucket->entry;
	bucket = bucket->next;
	}
	
	return NULL;
}

int
rasa_hash_table_remove(struct rasa_hash_table *table, const char *as_set_name)
{
	unsigned long hash;
	size_t idx;
	struct rasa_hash_bucket *bucket, *prev = NULL;
	
	if (!table || !as_set_name)
	return -1;
	
	hash = hash_asset(as_set_name);
	idx = hash % table->capacity;
	
	bucket = table->buckets[idx];
	while (bucket) {
	if (asset_cmp(bucket->entry->as_set_name, as_set_name) == 0) {
	    if (prev)
	        prev->next = bucket->next;
	    else
	        table->buckets[idx] = bucket->next;
	    
	    if (bucket->entry) {
	        if (bucket->entry->as_set_name)
	            free(bucket->entry->as_set_name);
	        if (bucket->entry->irr_source)
	            free(bucket->entry->irr_source);
	        if (bucket->entry->members)
	            free(bucket->entry->members);
	        if (bucket->entry->nested_sets) {
	            size_t i;
	            for (i = 0; i < bucket->entry->num_nested; i++)
	                if (bucket->entry->nested_sets[i])
	                    free(bucket->entry->nested_sets[i]);
	            free(bucket->entry->nested_sets);
	        }
	        free(bucket->entry);
	    }
	    
	    free(bucket);
	    table->count--;
	    return 0;
	}
	prev = bucket;
	bucket = bucket->next;
	}
	
	return -1;
}

size_t
rasa_hash_table_count(struct rasa_hash_table *table)
{
	return table ? table->count : 0;
}
