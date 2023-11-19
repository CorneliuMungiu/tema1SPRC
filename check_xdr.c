/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "check.h"

bool_t
xdr_request_authorization (XDR *xdrs, request_authorization *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->client_id, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->token, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_request_access_token (XDR *xdrs, request_access_token *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->access_token, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->regenerate_token, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_validate_delegated_action (XDR *xdrs, validate_delegated_action *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->operation_type, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->accessed_resource, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->token, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}
