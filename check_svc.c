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

UsersIdDB usersIdDatabase;

int main (int argc, char **argv)
{
	char path[255] = "tests/test1/userIDs.db"; 

    // Initialize the struct members
    usersIdDatabase.number_of_users = 0;
    usersIdDatabase.users = NULL;

    // Call the function
    allocate_users_id_db(path, &usersIdDatabase);

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
