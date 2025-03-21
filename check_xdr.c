/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "check.h"

bool_t
xdr_request_authorization (XDR *xdrs, request_authorization *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->client_id, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->auto_refresh))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_request_access_token (XDR *xdrs, request_access_token *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->access_token, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->refresh_token, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->validation_time))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->error, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_validate_delegated_action (XDR *xdrs, validate_delegated_action *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->operation_type, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->accessed_resource, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, ~0))
		 return FALSE;
	return TRUE;
}
