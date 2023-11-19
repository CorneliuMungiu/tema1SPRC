struct request_authorization {
    char* client_id;
    char* token;
};

struct request_access_token {
    char* access_token;
    char* regenerate_token;
};

struct validate_delegated_action {
    char* operation_type;
    char* accessed_resource;
    char* token;
};

program CHECKPROG {
    version CHECKVERS{
        string request_authorization_func(string) = 1;
        struct request_access_token request_access_token_func(struct request_authorization) = 2;
        string validate_delegated_action_func(struct validate_delegated_action) = 3;
        string approve_request_token_func(string) = 4;
    } = 1;
} = 0x1DBC0;