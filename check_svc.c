/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "check.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
checkprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char *request_authorization_func_1_arg;
		struct request_authorization request_access_token_func_1_arg;
		struct validate_delegated_action validate_delegated_action_func_1_arg;
		char *approve_request_token_func_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case request_authorization_func:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) request_authorization_func_1_svc;
		break;

	case request_access_token_func:
		_xdr_argument = (xdrproc_t) xdr_request_authorization;
		_xdr_result = (xdrproc_t) xdr_request_access_token;
		local = (char *(*)(char *, struct svc_req *)) request_access_token_func_1_svc;
		break;

	case validate_delegated_action_func:
		_xdr_argument = (xdrproc_t) xdr_validate_delegated_action;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) validate_delegated_action_func_1_svc;
		break;

	case approve_request_token_func:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) approve_request_token_func_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

void allocate_users_id_db(char* path, struct users_id_database *db) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", path);
        return;
    }
    // Read the number of IDs
    if (fscanf(file, "%u", &(db->number_of_users)) != 1) {
        fprintf(stderr, "Error reading the number of IDs.\n");
        fclose(file);
        return;
    }
    // Allocate memory for users array
    db->users = (char**)malloc(db->number_of_users * sizeof(char*));
    if (db->users == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return;
    }
    // Buffer to store each ID
    char id[20];
    // Read and store each ID
    for (unsigned int i = 0; i < db->number_of_users; ++i) {
        if (fscanf(file, "%19s", id) != 1) {
            fprintf(stderr, "Error reading ID number %u.\n", i + 1);
            free(db->users); // Free allocated memory
            fclose(file);
            return;
        }

        // Allocate memory for each user ID and copy the string
        db->users[i] = strdup(id);
        if (db->users[i] == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            // Cleanup: Free previously allocated memory
            for (unsigned int j = 0; j < i; ++j) {
                free(db->users[j]);
            }
            free(db->users);
            fclose(file);
            return;
        }
    }
    fclose(file);
}

void read_resources(char* path, struct users_id_database *db){
	FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", path);
        return;
    }
	if (fscanf(file, "%u", &(db->number_of_resources)) != 1) {
        fprintf(stderr, "Error reading the number of resources.\n");
        fclose(file);
        return;
    }

	// Allocate memory for resources array
    db->resources = (char**)malloc(db->number_of_resources * sizeof(char*));
    if (db->resources == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return;
    }

	char id[200];
    // Read and store each ID
    for (unsigned int i = 0; i < db->number_of_resources; ++i) {
        if (fscanf(file, "%199s", id) != 1) {
            fprintf(stderr, "Error reading resource %u.\n", i + 1);
            free(db->resources); // Free allocated memory
            fclose(file);
            return;
        }

        // Allocate memory for each user ID and copy the string
        db->resources[i] = strdup(id);
        if (db->resources[i] == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            // Cleanup: Free previously allocated memory
            for (unsigned int j = 0; j < i; ++j) {
                free(db->resources[j]);
            }
            free(db->resources);
            fclose(file);
            return;
        }
	}
}

int is_approval_in_resources(char* approval, struct users_id_database *db) {
	for (unsigned int i = 0; i < db->number_of_resources; i++) {
		if (strcmp(approval, db->resources[i]) == 0){
			return 1;
		}
	}
	return 0;
}

void reset_all_permisions(ApprovalsDB* approvals_database){
	approvals_database->read = 0;
	approvals_database->delete = 0;
	approvals_database->execute = 0;
	approvals_database->insert = 0;
	approvals_database->modify = 0;
}

void add_permisions(ApprovalsDB* approvals_database, char permision){
	if (permision == 'R'){
		approvals_database->read = 1;
	} else if (permision == 'D'){
		approvals_database->delete = 1;
	} else if (permision == 'X'){
		approvals_database->execute = 1;
	} else if (permision == 'I'){
		approvals_database->insert = 1;
	} else if (permision == 'M'){
		approvals_database->modify = 1;
	}
}

int is_permision_in_approval(ApprovalsDB* approvals_database_line, char* permision, int length){
	if (permision[strlen(permision) - 1] == '\n') {
			permision[strlen(permision) - 1] = '\0';
	}

	for(int i = 0; i < length; i++){
		if(approvals_database_line[i].name == NULL)
			return 0;
		if(strcmp(approvals_database_line[i].name, permision) == 0){
			return 1;
		}
	}
	return 0;
}

void add_permision_to_approvals(ApprovalsDB** approvals_database_line, char* permision, int length){
	// printf("aa\n");
	ApprovalsDB* approvals = *approvals_database_line;
	for(int i = 0; i < length; i++){
		// printf("bb\n");
		if (approvals[i].name == NULL){
			// printf("aa\n");
			// printf("%p\n",*approvals_database_line[i]->name);
			approvals[i].name = strdup(permision);
			return;
		}
	}
}

void add_excepted_permision(ApprovalsDB* approvals_database_line, struct users_id_database *db){
	if (approvals_database_line[0].name == NULL){
		// printf("===========\n");
		return;
	}

	for (int i = 0; i < db->number_of_resources; i++){
		if(!is_permision_in_approval(approvals_database_line, db->resources[i], db->number_of_resources)){
			// printf("%s\n",db->resources[i]);
			add_permision_to_approvals(&approvals_database_line, db->resources[i], db->number_of_resources);
		}
	}

	for (int i = 0; i < db->number_of_approvals_lines; i++) {
		for (int j = 0; j < db->number_of_approvals_colums; j ++) {

		}
	}
}

void read_approvals(char* path, struct users_id_database *db){
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
		db->approvals = realloc(db->approvals, (db->number_of_approvals_lines + 1) * sizeof(ApprovalsDB*));
		db->number_of_approvals_lines++;
        char *substring = strtok(line, ",");
		int counter = 0;
		db->approvals[db->number_of_approvals_lines - 1] = calloc(db->number_of_resources, sizeof(ApprovalsDB));
		for (int i = 0; i < db->number_of_resources; i++){
			reset_all_permisions(&db->approvals[db->number_of_approvals_lines - 1][i]);
		}
        while (substring != NULL) {
			if(is_approval_in_resources(substring, db)){
				db->approvals[db->number_of_approvals_lines - 1][counter].name = strdup(substring);
				substring = strtok(NULL, ",");
				for (unsigned int i = 0; i < strlen(substring); i++){
					add_permisions(&db->approvals[db->number_of_approvals_lines - 1][counter], substring[i]);
				}
			}
			counter++;
            substring = strtok(NULL, ",");
        }
    }
	for (int i = 0; i < db->number_of_approvals_lines; i++){
			add_excepted_permision(db->approvals[i], db);
	}
	db->number_of_approvals_colums = db->number_of_resources;
	for (int i = 0; i < db->number_of_approvals_lines; i++) {
		for (int j = 0; j < db->number_of_approvals_colums; j ++) {
			// if(&db->approvals[i][j].name == NULL){

				printf("%s ", db->approvals[i][j].name);
				printf("%d %d %d %d %d\n",db->approvals[i][j].delete, db->approvals[i][j].execute, db->approvals[i][j].insert, db->approvals[i][j].modify, db->approvals[i][j].read);
			// }
		}
		printf("\n");
	}
    fclose(file);
}


UsersIdDB usersIdDatabase;

int
main (int argc, char **argv)
{
	char path[255] = "tests/test1/userIDs.db"; 
	char path1[255] = "tests/test1/resources.db";
	char path2[255] = "tests/test1/approvals.db";

    // Initialize the struct members
    usersIdDatabase.number_of_users = 0;
	usersIdDatabase.number_of_authz_tokens = 0;
    usersIdDatabase.users = NULL;
	usersIdDatabase.authz_tokens = NULL;
	usersIdDatabase.resources = NULL;
	usersIdDatabase.number_of_resources = 0;
	usersIdDatabase.approvals = NULL;
	usersIdDatabase.number_of_approvals_lines = 0;
	usersIdDatabase.number_of_approvals_colums = 0;
	usersIdDatabase.signed_tokens_permisions = NULL;
	usersIdDatabase.number_of_signed_tokens_permisions = 0;
	usersIdDatabase.access_token_list = NULL;
	usersIdDatabase.number_of_access_tokens = 0;


    // Call the function
    allocate_users_id_db(path, &usersIdDatabase);
	read_resources(path1, &usersIdDatabase);
	read_approvals(path2, &usersIdDatabase);
	// printf("%d\n",is_approval_in_resources("UserDatas", &usersIdDatabase));
	// printf("%s\n",usersIdDatabase.approvals[0][0].name);


	register SVCXPRT *transp;

	pmap_unset (CHECKPROG, CHECKVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, CHECKPROG, CHECKVERS, checkprog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (CHECKPROG, CHECKVERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, CHECKPROG, CHECKVERS, checkprog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (CHECKPROG, CHECKVERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
