/*
 * RASA integration patch for expander.c
 * Replace the check_rasa_set_mode() function and update bgpq_expanded_macro_limit()
 */

/* Include the hash table header */
#ifdef HAVE_JANSSON
#include "rasa_hash.h"

/*
 * Process RASA-SET members by adding them to the expander
 */
static int
process_rasa_members(struct bgpq_expander *b, struct rasa_set_entry *rasa_set)
{
	size_t i;
	int count = 0;
	
	if (!rasa_set || !rasa_set->members)
		return 0;
	
	for (i = 0; i < rasa_set->num_members; i++) {
		uint32_t asn = rasa_set->members[i];
		char as_str[32];
		
		snprintf(as_str, sizeof(as_str), "AS%u", asn);
		
		if (bgpq_expander_add_as(b, as_str)) {
			SX_DEBUG(debug_expander, "RASA: Added AS%u\n", asn);
			count++;
		}
	}
	
	return count;
}

/*
 * Check if RASA-SET exists for an AS-SET and return fallback mode
 * Returns: -1 = no RASA-SET, 0 = irrFallback, 1 = irrLock, 2 = rasaOnly
 */
static int
check_rasa_set_mode(struct bgpq_expander *b, const char *asset,
    struct rasa_set_entry **rasa_set_out)
{
	struct rasa_set_entry *rasa_set;
	
	(void)b; /* Unused for now, may need for validation */
	
	rasa_set = rasa_lookup_set(asset);
	if (!rasa_set) {
		if (rasa_set_out)
			*rasa_set_out = NULL;
		return -1;
	}
	
	if (rasa_set_out)
		*rasa_set_out = rasa_set;
	
	return rasa_set->fallback_mode;
}
#endif
