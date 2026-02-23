#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../rasa.h"

static char* make_temp(const char*c){static char p[]="/tmp/r_XXXXXX";int fd=mkstemp(p);if(fd<0)return NULL;write(fd,c,strlen(c));close(fd);return strdup(p);}
static void clean_temp(char*p){if(p){unlink(p);free(p);}}

int main() {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\": [{\"rasa\": {\"authorized_as\": 64496, \"authorized_in\": [{\"entry\": {\"asset\": \"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\": [{\"rasa_set\": {\"as_set_name\": \"AS-TEST\", \"members\": [99999]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    
    printf("Loading RASA-AUTH from: %s\n", auth_path);
    printf("Loading RASA-SET from: %s\n", set_path);
    
    int rc1 = rasa_load_config(&auth_cfg, auth_path);
    printf("RASA-AUTH load: %d (enabled=%d)\n", rc1, auth_cfg.enabled);
    
    int rc2 = rasa_set_load_config(&set_cfg, set_path);
    printf("RASA-SET load: %d (enabled=%d)\n", rc2, set_cfg.enabled);
    
    int rc = rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result);
    printf("\nResults:\n");
    printf("  auth.authorized = %d (expected: 1)\n", auth_result.authorized);
    printf("  set.is_member = %d (expected: 0)\n", set_result.is_member);
    printf("  auth.reason = %s\n", auth_result.reason ? auth_result.reason : "NULL");
    printf("  set.reason = %s\n", set_result.reason ? set_result.reason : "NULL");
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
    
    return (auth_result.authorized == 1 && set_result.is_member == 0) ? 0 : 1;
}
