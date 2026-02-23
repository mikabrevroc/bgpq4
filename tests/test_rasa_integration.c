/*
 * Copyright (c) 2025 RASA Project
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../extern.h"
#include "../rasa.h"

static int tests_run = 0, tests_passed = 0, tests_failed = 0;
#define RUN_TEST(n) do { int p=tests_failed; printf("  %s... ",#n); tests_run++; test_##n(); if(tests_failed==p){tests_passed++;printf("OK\n");}else{printf("FAIL\n");}}while(0)
#define ASSERT(c) do{if(!(c)){tests_failed++;return;}}while(0)

void test_expansion(){struct bgpq_expander e={0};ASSERT(bgpq_expander_init(&e,AF_INET));expander_freeall(&e);}
void test_asset(){struct bgpq_expander e={0};ASSERT(bgpq_expander_init(&e,AF_INET));
#ifdef HAVE_JANSSON
e.current_asset=strdup("AS-T");ASSERT(e.current_asset);free(e.current_asset);e.current_asset=NULL;
#endif
expander_freeall(&e);}

int main(){printf("\nRASA Integration Tests\n\n");RUN_TEST(expansion);RUN_TEST(asset);printf("\n%d/%d passed\n\n",tests_passed,tests_run);return tests_failed>0?1:0;}
