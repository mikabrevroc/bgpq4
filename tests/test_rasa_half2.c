    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_birectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    /* No RASA-SET data - defaults to allow */
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_only_set(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    /* No RASA-AUTH - defaults to allow */
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_neither_authorize(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-OTHER\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-OTHER\",\"members\":[64497]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1); /* No RASA-SET for AS-TEST */
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_auth_denies_set_allows(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* AS 64496 NOT authorized in AS-TEST per RASA-AUTH */
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    /* But AS 64496 IS in AS-TEST per RASA-SET */
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    /* Per strict bidirectional: both must agree */
    /* AS does not authorize, so authorization fails */
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_auth_allows_set_denies(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* AS 64496 authorized in AS-TEST per RASA-AUTH */
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    /* But AS 64496 NOT in AS-TEST per RASA-SET */
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64497]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 0);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_multiple_asns_mixed(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,64497,64498]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* 64496: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64497: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64497, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64498: only RASA-SET */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64498, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


void test_bidirectional_no_configs(void) {
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* No configs loaded - both default to allow */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
}

void test_bidirectional_null_auth_result(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[]}";
    const char *set_json = "{\"rasa_sets\":[]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_birectional("AS-TEST", 64496, NULL, &set_result), -1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_null_set_result(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    
    const char *auth_json = "{\"rasas\":[]}";
    const char *set_json = "{\"rasa_sets\":[]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, NULL), -1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_32bit_asn(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4200000000,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4200000000]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 4200000000U, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_multiple_assets(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST1\"}},{\"entry\":{\"asset\":\"AS-TEST2\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST1\",\"members\":[64496]}},{\"rasa_set\":{\"as_set_name\":\"AS-TEST2\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST1", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST2", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_wrong_asset_both_loaded(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-AUTH\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-SET\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Check against asset that exists in neither */
    ASSERT_EQ(rasa_verify_bidirectional("AS-OTHER", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1); /* No RASA-SET for AS-OTHER */
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_empty_members_vs_empty_auth(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 0);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_asn_zero(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":0,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[0]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 0, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_special_chars_asset(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS2914:AS-GLOBAL\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS2914:AS-GLOBAL\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS2914:AS-GLOBAL", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_large_asn_16bit_boundary(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":65535,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[65535]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 65535, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


void test_bidirectional_many_asns(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    char auth_json[4096];
    char set_json[4096];
    
    strcpy(auth_json, "{\"rasas\":[");
    strcpy(set_json, "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-BULK\",\"members\":[");
    
    for (int i = 0; i < 50; i++) {
        if (i > 0) {
            strcat(auth_json, ",");
            strcat(set_json, ",");
        }
        char entry[128];
        snprintf(entry, sizeof(entry), "{\"rasa\":{\"authorized_as\":%d,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-BULK\"}}]}}", 64496 + i);
        strcat(auth_json, entry);
        
        char num[16];
        snprintf(num, sizeof(num), "%d", 64496 + i);
        strcat(set_json, num);
    }
    strcat(auth_json, "]}");
    strcat(set_json, "]}}]}");
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 64545, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_partial_overlap(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Auth has ASNs 64496-64505 */
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}},{\"rasa\":{\"authorized_as\":64498,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}},{\"rasa\":{\"authorized_as\":64499,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}},{\"rasa\":{\"authorized_as\":64500,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    /* Set has ASNs 64498-64502 */
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64498,64499,64500,64501,64502]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* 64498-64500: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64498, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64496: only auth */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 0);
    
    /* 64501: only set */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64501, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_null_asset(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional(NULL, 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_case_sensitive_asset(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Different case - should not match */
    ASSERT_EQ(rasa_verify_bidirectional("as-test", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_complex_scenario(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Complex scenario with multiple AS-SETs and ASNs */
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}},{\"entry\":{\"asset\":\"AS-UNIQUE1\"}}]}},{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-SHARED\"}}]}},{\"rasa\":{\"authorized_as\":64498,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-UNIQUE2\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-SHARED\",\"members\":[64496,64497,64499]}},{\"rasa_set\":{\"as_set_name\":\"AS-UNIQUE1\",\"members\":[64496]}},{\"rasa_set\":{\"as_set_name\":\"AS-UNIQUE2\",\"members\":[64498,64500]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* 64496 in AS-SHARED: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-SHARED", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64496 in AS-UNIQUE1: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-UNIQUE1", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64497 in AS-SHARED: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-SHARED", 64497, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64498 in AS-UNIQUE2: both authorize */
    ASSERT_EQ(rasa_verify_bidirectional("AS-UNIQUE2", 64498, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64499 in AS-SHARED: only set (not in auth) */
    ASSERT_EQ(rasa_verify_bidirectional("AS-SHARED", 64499, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* 64500 in AS-UNIQUE2: only set */
    ASSERT_EQ(rasa_verify_bidirectional("AS-UNIQUE2", 64500, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_minimal_configs(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[]}";
    const char *set_json = "{\"rasa_sets\":[]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Empty configs - default allow */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


void test_bidirectional_different_asn_in_each(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Auth has ASN 64496, Set has ASN 64497 */
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64497]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Check 64496 - authorized by auth but not in set */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 0);
    
    /* Check 64497 - in set but not authorized by auth */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64497, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_same_config_file(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Both configs in one file - but loaded separately */
    const char *combined_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}],\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    char *path = make_temp(combined_json);
    ASSERT(path);
    
    rasa_load_config(&auth_cfg, path);
    rasa_set_load_config(&set_cfg, path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(path);
}

void test_bidirectional_large_scale(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Build large auth config */
    char auth_json[8192];
    char set_json[8192];
    
    strcpy(auth_json, "{\"rasas\":[");
    strcpy(set_json, "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-BULK\",\"members\":[");
    
    for (int i = 0; i < 100; i++) {
        if (i > 0) {
            strcat(auth_json, ",");
            strcat(set_json, ",");
        }
        char entry[128];
        snprintf(entry, sizeof(entry), "{\"rasa\":{\"authorized_as\":%d,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-BULK\"}}]}}", 64496 + i);
        strcat(auth_json, entry);
        
        char num[16];
        snprintf(num, sizeof(num), "%d", 64496 + i);
        strcat(set_json, num);
    }
    strcat(auth_json, "]}");
    strcat(set_json, "]}}]}");
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Test first, middle, and last */
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 64545, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 64595, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* Test outside range */
    ASSERT_EQ(rasa_verify_bidirectional("AS-BULK", 65500, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 0);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_reload_configs(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json1 = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json1 = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496]}}]}";
    
    const char *auth_json2 = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64497,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST2\"}}]}}]}";
    const char *set_json2 = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST2\",\"members\":[64497]}}]}";
    
    char *auth_path1 = make_temp(auth_json1);
    char *set_path1 = make_temp(set_json1);
    char *auth_path2 = make_temp(auth_json2);
    char *set_path2 = make_temp(set_json2);
    
    ASSERT(auth_path1 && set_path1 && auth_path2 && set_path2);
    
    /* First load */
    rasa_load_config(&auth_cfg, auth_path1);
    rasa_set_load_config(&set_cfg, set_path1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* Reload with different configs */
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    
    rasa_load_config(&auth_cfg, auth_path2);
    rasa_set_load_config(&set_cfg, set_path2);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST2", 64497, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    /* Old data should not work */
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 0);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path1);
    clean_temp(set_path1);
    clean_temp(auth_path2);
    clean_temp(set_path2);
}

void test_bidirectional_extra_json_fields(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"version\":\"1.0\",\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}],\"metadata\":{\"created\":\"2025-01-01\"}}}],\"extra\":\"ignored\"}";
    const char *set_json = "{\"version\":\"1.0\",\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496],\"metadata\":{\"owner\":\"AS64496\"}}}],\"extra\":\"ignored\"}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64496, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_default_allow_behavior(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    /* Load empty configs - should default allow */
    const char *auth_json = "{\"rasas\":[]}";
    const char *set_json = "{\"rasa_sets\":[]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    /* Any ASN should be allowed with empty configs */
    ASSERT_EQ(rasa_verify_bidirectional("AS-ANY", 12345, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-OTHER", 99999, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


/* ============================================
 * Additional Edge Case Tests
 * ============================================ */

void test_rasa_auth_very_large_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Maximum 32-bit unsigned value */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4294967295,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(4294967295U, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_set_very_large_asn(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4294967295]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 4294967295U, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_auth_private_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Private ASN range */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64512,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64512, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_set_private_asn(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64512,65534]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64512, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 65534, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_auth_reserved_asn(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* ASN 23456 is reserved for AS_TRANS */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":23456,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(23456, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_auth_nested_arrays(void) {
    struct rasa_config cfg = {0};
    struct rasa_auth result = {0};
    /* Test handling of nested arrays that might cause issues */
    const char *json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64496,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}},{\"entry\":{\"asset\":\"AS-TEST2\"}}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    ASSERT_EQ(rasa_check_auth(64496, "AS-TEST2", &result), 0);
    ASSERT_EQ(result.authorized, 1);
    rasa_free_config(&cfg);
    clean_temp(path);
}

void test_rasa_set_mixed_types_in_members(void) {
    struct rasa_set_config cfg = {0};
    struct rasa_set_membership result = {0};
    /* Test that non-integer members are skipped gracefully */
    const char *json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64496,\"string\",true,false,null,64497,[],{}]}}]}";
    char *path = make_temp(json);
    ASSERT(path);
    rasa_set_load_config(&cfg, path);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64496, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    ASSERT_EQ(rasa_check_set_membership("AS-TEST", 64497, &result), 0);
    ASSERT_EQ(result.is_member, 1);
    rasa_set_free_config(&cfg);
    clean_temp(path);
}

void test_bidirectional_very_large_asn(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":4294967295,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[4294967295]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 4294967295U, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_private_asns(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":64512,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[64512]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 64512, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}

void test_bidirectional_as_trans(void) {
    struct rasa_config auth_cfg = {0};
    struct rasa_set_config set_cfg = {0};
    struct rasa_auth auth_result = {0};
    struct rasa_set_membership set_result = {0};
    
    const char *auth_json = "{\"rasas\":[{\"rasa\":{\"authorized_as\":23456,\"authorized_in\":[{\"entry\":{\"asset\":\"AS-TEST\"}}]}}]}";
    const char *set_json = "{\"rasa_sets\":[{\"rasa_set\":{\"as_set_name\":\"AS-TEST\",\"members\":[23456]}}]}";
    
    char *auth_path = make_temp(auth_json);
    char *set_path = make_temp(set_json);
    ASSERT(auth_path && set_path);
    
    rasa_load_config(&auth_cfg, auth_path);
    rasa_set_load_config(&set_cfg, set_path);
    
    ASSERT_EQ(rasa_verify_bidirectional("AS-TEST", 23456, &auth_result, &set_result), 0);
    ASSERT_EQ(auth_result.authorized, 1);
    ASSERT_EQ(set_result.is_member, 1);
    
    rasa_free_config(&auth_cfg);
    rasa_set_free_config(&set_cfg);
    clean_temp(auth_path);
    clean_temp(set_path);
}


/* ============================================
 * Main Test Runner
 * ============================================ */

int main(void)
{
    printf("\n========================================\n");
    printf("RASA Comprehensive Test Suite\n");
    printf("========================================\n\n");
    
    printf("RASA-AUTH Tests (40 scenarios):\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_auth_load_valid);
    RUN_TEST(rasa_auth_load_null_filename);
    RUN_TEST(rasa_auth_load_invalid_json);
    RUN_TEST(rasa_auth_load_empty_object);
    RUN_TEST(rasa_auth_load_missing_rasas);
    RUN_TEST(rasa_auth_check_null_result);
    RUN_TEST(rasa_auth_check_no_config);
    RUN_TEST(rasa_auth_check_single_asn);
    RUN_TEST(rasa_auth_check_wrong_asn);
    RUN_TEST(rasa_auth_check_wrong_asset);
    RUN_TEST(rasa_auth_multiple_asns);
    RUN_TEST(rasa_auth_multiple_assets_single_asn);
    RUN_TEST(rasa_auth_empty_authorized_in);
    RUN_TEST(rasa_auth_no_authorized_in_key);
    RUN_TEST(rasa_auth_special_chars_in_asset);
    RUN_TEST(rasa_auth_32bit_asn);
    RUN_TEST(rasa_auth_asn_zero);
    RUN_TEST(rasa_auth_large_asn_16bit_max);
    RUN_TEST(rasa_auth_large_asn_16bit_plus_one);
    RUN_TEST(rasa_auth_duplicate_entries);
    RUN_TEST(rasa_auth_null_asset);
    RUN_TEST(rasa_auth_empty_asset);
    RUN_TEST(rasa_auth_whitespace_asset);
    RUN_TEST(rasa_auth_case_sensitive);
    RUN_TEST(rasa_auth_malformed_entry);
    RUN_TEST(rasa_auth_missing_asset_field);
    RUN_TEST(rasa_auth_non_integer_asn);
    RUN_TEST(rasa_auth_negative_asn);
    RUN_TEST(rasa_auth_reuse_config_struct);
    RUN_TEST(rasa_auth_free_null_config);
    RUN_TEST(rasa_auth_many_asns);
    RUN_TEST(rasa_auth_many_assets);
    RUN_TEST(rasa_auth_asn_not_in_any_rasa);
    RUN_TEST(rasa_auth_different_assets_different_asns);
    RUN_TEST(rasa_auth_overlapping_authorizations);
    RUN_TEST(rasa_auth_long_asset_name);
    RUN_TEST(rasa_auth_extra_fields_ignored);
    RUN_TEST(rasa_auth_minimal_valid);
    RUN_TEST(rasa_auth_propagation_field);
    RUN_TEST(rasa_auth_nested_entry_format);
    
    printf("\nRASA-SET Tests (40 scenarios):\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_set_load_valid);
    RUN_TEST(rasa_set_load_null_filename);
    RUN_TEST(rasa_set_load_invalid_json);
    RUN_TEST(rasa_set_check_no_config);
    RUN_TEST(rasa_set_check_null_result);
    RUN_TEST(rasa_set_check_single_member);
    RUN_TEST(rasa_set_check_multiple_members);
    RUN_TEST(rasa_set_check_wrong_set_name);
    RUN_TEST(rasa_set_check_empty_members);
    RUN_TEST(rasa_set_check_no_members_key);
    RUN_TEST(rasa_set_multiple_sets);
    RUN_TEST(rasa_set_32bit_asn_member);
    RUN_TEST(rasa_set_special_chars_name);
    RUN_TEST(rasa_set_asn_zero);
    RUN_TEST(rasa_set_large_asn_16bit_max);
    RUN_TEST(rasa_set_large_asn_16bit_plus_one);
    RUN_TEST(rasa_set_duplicate_members);
    RUN_TEST(rasa_set_null_set_name);
    RUN_TEST(rasa_set_empty_set_name);
    RUN_TEST(rasa_set_case_sensitive);
    RUN_TEST(rasa_set_missing_rasa_sets_key);
    RUN_TEST(rasa_set_missing_as_set_name);
    RUN_TEST(rasa_set_non_integer_member);
    RUN_TEST(rasa_set_reuse_config_struct);
    RUN_TEST(rasa_set_free_null_config);
    RUN_TEST(rasa_set_many_sets);
    RUN_TEST(rasa_set_many_members);
    RUN_TEST(rasa_set_asn_not_in_any_set);
    RUN_TEST(rasa_set_whitespace_in_name);
    RUN_TEST(rasa_set_long_set_name);
    RUN_TEST(rasa_set_extra_fields_ignored);
    RUN_TEST(rasa_set_minimal_valid);
    RUN_TEST(rasa_set_nested_sets_declaration);
    RUN_TEST(rasa_set_containing_as_field);
    RUN_TEST(rasa_set_load_empty_object);
    RUN_TEST(rasa_set_negative_member_asn);
    RUN_TEST(rasa_set_large_member_list_mixed);
    RUN_TEST(rasa_set_multiple_same_asn_different_sets);
    RUN_TEST(rasa_set_members_array_with_nulls);
    RUN_TEST(rasa_set_boolean_in_members);
    
    printf("\nBidirectional Verification Tests (30 scenarios):\n");
    printf("----------------------------------------\n");
    RUN_TEST(bidirectional_both_authorize);
    RUN_TEST(bidirectional_only_auth);
    RUN_TEST(bidirectional_only_set);
    RUN_TEST(bidirectional_neither_authorize);
    RUN_TEST(bidirectional_auth_denies_set_allows);
    RUN_TEST(bidirectional_auth_allows_set_denies);
    RUN_TEST(bidirectional_multiple_asns_mixed);
    RUN_TEST(bidirectional_no_configs);
    RUN_TEST(bidirectional_null_auth_result);
    RUN_TEST(bidirectional_null_set_result);
    RUN_TEST(bidirectional_32bit_asn);
    RUN_TEST(bidirectional_multiple_assets);
    RUN_TEST(bidirectional_wrong_asset_both_loaded);
    RUN_TEST(bidirectional_empty_members_vs_empty_auth);
    RUN_TEST(bidirectional_asn_zero);
    RUN_TEST(bidirectional_special_chars_asset);
    RUN_TEST(bidirectional_large_asn_16bit_boundary);
    RUN_TEST(bidirectional_many_asns);
    RUN_TEST(bidirectional_partial_overlap);
    RUN_TEST(bidirectional_null_asset);
    RUN_TEST(bidirectional_case_sensitive_asset);
    RUN_TEST(bidirectional_complex_scenario);
    RUN_TEST(bidirectional_minimal_configs);
    RUN_TEST(bidirectional_different_asn_in_each);
    RUN_TEST(bidirectional_same_config_file);
    RUN_TEST(bidirectional_large_scale);
    RUN_TEST(bidirectional_reload_configs);
    RUN_TEST(bidirectional_extra_json_fields);
    RUN_TEST(bidirectional_default_allow_behavior);
    
    printf("\nAdditional Edge Case Tests:\n");
    printf("----------------------------------------\n");
    RUN_TEST(rasa_auth_very_large_asn);
    RUN_TEST(rasa_set_very_large_asn);
    RUN_TEST(rasa_auth_private_asn);
    RUN_TEST(rasa_set_private_asn);
    RUN_TEST(rasa_auth_reserved_asn);
    RUN_TEST(rasa_auth_nested_arrays);
    RUN_TEST(rasa_set_mixed_types_in_members);
    RUN_TEST(bidirectional_very_large_asn);
    RUN_TEST(bidirectional_private_asns);
    RUN_TEST(bidirectional_as_trans);
    
    printf("\n========================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("========================================\n\n");
    
    return tests_failed > 0 ? 1 : 0;
}
