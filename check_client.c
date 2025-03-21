/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "check.h"

struct clients_list {
	char *client_id;
	char *auth_token;
	char *access_token;
};

typedef struct clients_list ClientsList;

struct clients_database{
	ClientsList *clients_list;
	unsigned int number_of_clients;
};

typedef struct clients_database ClientsDB;

ClientsDB db;
char *path;


void
checkprog_1(char *host)
{
	CLIENT *clnt;
	char * *result_1;
	char * request_authorization_func_1_arg;
	request_authorization_func_1_arg = calloc(20, sizeof(char));
	request_authorization_func_1_arg = "X5B0TWjmeNtU3vd";
	char * request_authorization_func_2_arg;
	request_authorization_func_2_arg = calloc(20, sizeof(char));
	request_authorization_func_2_arg = "74xaLqdTDdHZ9ey";
	struct request_access_token  *result_2;
	struct request_authorization request_access_token_func_1_arg;
	char * *result_3;
	struct validate_delegated_action  validate_delegated_action_func_1_arg;
	char * *result_4;
	char * approve_request_token_func_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, CHECKPROG, CHECKVERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	
	FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[LINE_LENGTH];

	 while (fgets(line, sizeof(line), file) != NULL) {
		if (line[strlen(line) - 1] == '\n'){
			line[strlen(line) - 1] = '\0';
		}
		char *client_id = strtok(line, ",");
		char *action = strtok(NULL, ",");
		char *resource = strtok(NULL, ",");

		if(strcmp(action, "REQUEST") == 0){
			ClientsList *find = NULL;
			for(int i = 0; i < db.number_of_clients; i++){
				if(strcmp(client_id, db.clients_list[i].client_id) == 0){
					find = &db.clients_list[i];
				}
			}
			if (find != NULL) {
				char **auth_token = request_authorization_func_1(&client_id, clnt);
				if (auth_token == (char **) NULL) {
					clnt_perror (clnt, "call failed");
				}
				find->auth_token = strdup(*auth_token);
				char **signed_auth_token_return = approve_request_token_func_1(&find->auth_token, clnt);
				if (signed_auth_token_return == (char **) NULL) {
					clnt_perror (clnt, "call failed");
				}
				char *signed_auth_token = strdup(*signed_auth_token_return);
				struct request_authorization request_authorization;
				request_authorization.client_id = strdup(client_id);
				request_authorization.token = signed_auth_token;
				request_authorization.auto_refresh = atoi(resource);
				struct request_access_token *request_access_token = request_access_token_func_1(&request_authorization, clnt);
				if (request_access_token == (struct request_access_token *) NULL) {
					clnt_perror (clnt, "call failed");
				}
				if (strcmp(request_access_token->error, "") != 0) {
					printf("%s\n", request_access_token->error);
					continue;
				}
				find->access_token = request_access_token->access_token;
				printf("%s -> %s", find->auth_token, find->access_token);
				if(strcmp(request_access_token->refresh_token, "") != 0){
					printf(",%s\n",request_access_token->refresh_token);
				} else {
					printf("\n");
				}
				continue;
			}

			char **auth_token_return = request_authorization_func_1(&client_id, clnt);
			if (auth_token_return == (char **) NULL) {
					clnt_perror (clnt, "call failed");
			}
			char* auth_token = strdup(*auth_token_return);
			if(strcmp(auth_token,"USER_NOT_FOUND") == 0){
				printf("%s\n", auth_token);
				continue;
			}
			char **signed_auth_token_return = approve_request_token_func_1(auth_token_return, clnt);
			if (signed_auth_token_return == (char **) NULL) {
					clnt_perror (clnt, "call failed");
			}
			char *signed_auth_token = strdup(*signed_auth_token_return);
			struct request_authorization request_authorization;
			request_authorization.client_id = strdup(client_id);
			request_authorization.token = signed_auth_token;
			request_authorization.auto_refresh = atoi(resource);
			struct request_access_token *request_access_token = request_access_token_func_1(&request_authorization, clnt);
			if (request_access_token == (struct request_access_token *) NULL) {
					clnt_perror (clnt, "call failed");
			}
			if (strcmp(request_access_token->error, "") != 0) {
				printf("%s\n", request_access_token->error);
				continue;
			}
			db.clients_list = realloc(db.clients_list, (db.number_of_clients + 1) * sizeof(ClientsList));
			db.clients_list[db.number_of_clients].client_id = strdup(client_id);
			db.clients_list[db.number_of_clients].auth_token = auth_token;
			db.clients_list[db.number_of_clients].access_token = request_access_token->access_token;
			db.number_of_clients++;
			if(strcmp(request_access_token->error, "") != 0){
				printf("%s\n",request_access_token->error);
				continue;
			}
			printf("%s -> %s", db.clients_list[db.number_of_clients -1].auth_token, db.clients_list[db.number_of_clients -1 ].access_token);
			if(strcmp(request_access_token->refresh_token, "") != 0){
					printf(",%s\n",request_access_token->refresh_token);
			} else {
					printf("\n");
			}
		} else {
			// printf("asdadasdasdasdasdasdasd\n");
			int exits = 0;
			for(int i = 0; i < db.number_of_clients; i++){
				if(strcmp(db.clients_list[i].client_id, client_id) == 0) {
					exits = 1;
					struct validate_delegated_action validate_delegated_action_arg;
					validate_delegated_action_arg.accessed_resource = strdup(resource);
					validate_delegated_action_arg.operation_type = strdup(action);
					validate_delegated_action_arg.token = strdup(db.clients_list[i].access_token);
					result_2 = validate_delegated_action_func_1(&validate_delegated_action_arg, clnt);

					if (result_2 == (struct request_access_token *) NULL) {
						clnt_perror (clnt, "call failed");
					}
					if (strcmp(result_2->access_token, "") != 0) {
						db.clients_list[i].access_token = strdup(result_2->access_token);
					}
					
					printf("%s\n", result_2->error);
				}
			}
			if (!exits) {
				// printf("nu exista user\n");
				struct validate_delegated_action validate_delegated_action_arg;
				validate_delegated_action_arg.accessed_resource = strdup(resource);
				validate_delegated_action_arg.operation_type = strdup(action);
				validate_delegated_action_arg.token = strdup(client_id);
				result_2 = validate_delegated_action_func_1(&validate_delegated_action_arg, clnt);

				if (result_2 == (struct request_access_token *) NULL) {
					clnt_perror (clnt, "call failed");
				}
				printf("%s\n", result_2->error);
			}
		}
	 }
	
	

	// result_1 = request_authorization_func_1(&request_authorization_func_1_arg, clnt);
	// result_1 = request_authorization_func_1(&request_authorization_func_2_arg, clnt);
	// if (result_1 == (char **) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
	// result_3 = validate_delegated_action_func_1(&validate_delegated_action_func_1_arg, clnt);
	// if (result_3 == (char **) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
	// approve_request_token_func_1_arg = *result_1;
	// result_4 = approve_request_token_func_1(&approve_request_token_func_1_arg, clnt);
	// approve_request_token_func_1_arg = "bababa";
	// result_4 = approve_request_token_func_1(&approve_request_token_func_1_arg, clnt);
	// approve_request_token_func_1_arg = "lalalal";
	// result_4 = approve_request_token_func_1(&approve_request_token_func_1_arg, clnt);
	// approve_request_token_func_1_arg = "lalalal";
	// result_4 = approve_request_token_func_1(&approve_request_token_func_1_arg, clnt);
	// printf("%s\n", *result_4);
	// if (result_4 == (char **) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
	// request_access_token_func_1_arg.client_id = strdup("X5B0TWjmeNtU3vd");
	// request_access_token_func_1_arg.token = *result_4;
	// result_2 = request_access_token_func_1(&request_access_token_func_1_arg, clnt);

	// if (result_2 == (struct request_access_token *) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	db.number_of_clients = 0;
	char *host;
	path = calloc(LINE_LENGTH, sizeof(char));
	strcpy(path, argv[2]);
	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	checkprog_1 (host);
exit (0);
}
