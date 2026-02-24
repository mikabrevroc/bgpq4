#ifdef HAVE_JANSSON

#include "rasa.h"
#include "rasa_hash.h"

extern int debug_expander;
extern int pipelining;

int rasa_load_sets_from_json(const char *filename);
struct rasa_set_entry *rasa_lookup_set(const char *as_set_name);

static int
process_rasa_members(struct bgpq_expander *b, struct rasa_set_entry *rasa_set,
    const char *asset)
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
            SX_DEBUG(debug_expander, "RASA: Added AS%u from RASA-SET %s\n",
                asn, asset);
            count++;
        }
    }
    
    return count;
}

static int
expand_rasa_nested(struct bgpq_expander *b, struct rasa_set_entry *rasa_set,
    struct request *req, int depth)
{
    size_t i;
    int total = 0;
    
    if (!rasa_set || !rasa_set->nested_sets)
        return 0;
    
    for (i = 0; i < rasa_set->num_nested; i++) {
        char *nested = rasa_set->nested_sets[i];
        int result;
        
        SX_DEBUG(debug_expander, "RASA: Expanding nested set %s from %s\n",
            nested, rasa_set->as_set_name);
        
        result = bgpq_expand_single_macro(b, nested, req, depth + 1);
        total += result;
    }
    
    return total;
}

static int
handle_irrlock_mode(struct bgpq_expander *b, struct rasa_set_entry *rasa_set,
    const char *asset, struct request *req, int depth)
{
    char *source;
    
    if (!rasa_set->irr_source || strlen(rasa_set->irr_source) == 0) {
        sx_report(SX_ERROR, "RASA-SET %s has irrLock mode but no irr_source\n",
            asset);
        return 0;
    }
    
    if (rasa_set->num_members > 0) {
        sx_report(SX_WARN, "RASA-SET %s irrLock mode has %zu members (should be empty)\n",
            asset, rasa_set->num_members);
    }
    
    SX_DEBUG(debug_expander, "RASA-SET: %s locked to %s\n",
        asset, rasa_set->irr_source);
    
    source = strdup(rasa_set->irr_source);
    
    if (pipelining) {
        bgpq_pipeline(b, NULL, NULL, "!s%s\n", source);
    } else {
        bgpq_expand_irrd(b, NULL, NULL, "!s%s\n", source);
    }
    
    free(source);
    
    return bgpq_expand_single_macro(b, asset, req, depth);
}

static int
handle_rasaonly_mode(struct bgpq_expander *b, struct rasa_set_entry *rasa_set,
    const char *asset, struct request *req, int depth)
{
    int member_count = 0;
    int nested_count = 0;
    
    if (rasa_set->num_members == 0 && rasa_set->num_nested == 0) {
        SX_DEBUG(debug_expander, "RASA-SET: %s rasaOnly mode with empty members (authoritative empty)\n",
            asset);
        return 0;
    }
    
    SX_DEBUG(debug_expander, "RASA-SET: %s in rasaOnly mode, using RASA data only\n", asset);
    
    member_count = process_rasa_members(b, rasa_set, asset);
    
    nested_count = expand_rasa_nested(b, rasa_set, req, depth);
    
    SX_DEBUG(debug_expander, "RASA-SET: %s rasaOnly mode added %d members, %d from nested\n",
        asset, member_count, nested_count);
    
    return member_count + nested_count;
}

static int
handle_irrfallback_mode(struct bgpq_expander *b, struct rasa_set_entry *rasa_set,
    const char *asset, struct request *req, int depth)
{
    int rasa_count = 0;
    int irr_count = 0;
    
    SX_DEBUG(debug_expander, "RASA-SET: %s in irrFallback mode, merging RASA and IRR\n", asset);
    
    rasa_count = process_rasa_members(b, rasa_set, asset);
    rasa_count += expand_rasa_nested(b, rasa_set, req, depth);
    
    irr_count = bgpq_expand_single_macro(b, asset, req, depth);
    
    SX_DEBUG(debug_expander, "RASA-SET: %s irrFallback mode added %d from RASA, %d from IRR\n",
        asset, rasa_count, irr_count);
    
    return rasa_count + irr_count;
}

int
bgpq_expand_with_rasa(struct bgpq_expander *b, const char *asset,
    struct request *req, int depth)
{
    struct rasa_set_entry *rasa_set;
    
    rasa_set = rasa_lookup_set(asset);
    
    if (!rasa_set) {
        SX_DEBUG(debug_expander > 2, "No RASA-SET for %s, using IRR only\n", asset);
        return bgpq_expand_single_macro(b, asset, req, depth);
    }
    
    switch (rasa_set->fallback_mode) {
    case RASA_FALLBACK_MODE_IRR_LOCK:
        return handle_irrlock_mode(b, rasa_set, asset, req, depth);
        
    case RASA_FALLBACK_MODE_RASA_ONLY:
        return handle_rasaonly_mode(b, rasa_set, asset, req, depth);
        
    case RASA_FALLBACK_MODE_IRR_FALLBACK:
    default:
        return handle_irrfallback_mode(b, rasa_set, asset, req, depth);
    }
}

#endif
