struct request_authorization {
    string client_id<>;
    string token<>;
};

struct request_access_token {
    string access_token<>;
    string refresh_token<>;
    int validation_time;
    string error<>;
};

struct validate_delegated_action {
    string operation_type<>;
    string accessed_resource<>;
    string token<>;
};

program CHECKPROG {
    version CHECKVERS{
        string request_authorization_func(string) = 1;
        struct request_access_token request_access_token_func(struct request_authorization) = 2;
        string validate_delegated_action_func(struct validate_delegated_action) = 3;
        string approve_request_token_func(string) = 4;
    } = 1;
} = 0x1DBC0;