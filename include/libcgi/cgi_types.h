/*******************************************************************//**
 *	@file		libcgi/cgi_types.h
 *
 *	@brief		Type definitions for libcgi.
 *
 *	@author		Alexander Dahl <post@lespocky.de>
 *
 *	SPDX-License-Identifier: LGPL-2.1+
 *	License-Filename: LICENSES/LGPL-2.1.txt
 *
 *	@copyright	2018 Alexander Dahl <post@lespocky.de>
 **********************************************************************/

#ifndef CGI_TYPES_H
#define CGI_TYPES_H

/**
 * HTTP status codes.
 *
 * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
 */
enum cgi_http_status_code {
	HTTP_STATUS_CONTINUE			= 100,
	HTTP_STATUS_SWITCHING_PROTOCOLS	= 101,

	HTTP_STATUS_OK					= 200,
	HTTP_STATUS_CREATED				= 201,
	HTTP_STATUS_ACCEPTED			= 202,
	HTTP_STATUS_NON_AUTH_INFO		= 203,
	HTTP_STATUS_NO_CONTENT			= 204,
	HTTP_STATUS_RESET_CONTENT		= 205,
	HTTP_STATUS_PARTIAL_CONTENT		= 206,

	HTTP_STATUS_MULTIPLE_CHOICES	= 300,
	HTTP_STATUS_MOVED_PERMANENTLY	= 301,
	HTTP_STATUS_FOUND				= 302,
	HTTP_STATUS_SEE_OTHER			= 303,
	HTTP_STATUS_NOT_MODIFIED		= 304,
	HTTP_STATUS_USE_PROXY			= 305,
	HTTP_STATUS_TEMPORARY_REDIRECT	= 307,

	HTTP_STATUS_BAD_REQUEST			= 400,
	HTTP_STATUS_UNAUTHORIZED		= 401,
	HTTP_STATUS_PAYMENT_REQUIRED	= 402,
	HTTP_STATUS_FORBIDDEN			= 403,
	HTTP_STATUS_NOT_FOUND			= 404,
	HTTP_STATUS_METHOD_NOT_ALLOWED	= 405,
	HTTP_STATUS_NOT_ACCEPTABLE		= 406,
	HTTP_STATUS_PROXY_AUTH_REQUIRED	= 407,
	HTTP_STATUS_REQUEST_TIMEOUT		= 408,
	HTTP_STATUS_CONFLICT			= 409,
	HTTP_STATUS_GONE				= 410,
	HTTP_STATUS_LENGTH_REQUIRED		= 411,
	HTTP_STATUS_PRECONDITION_FAILED	= 412,
	HTTP_STATUS_REQ_ENT_TOO_LARGE	= 413,
	HTTP_STATUS_REQ_URI_TOO_LONG	= 414,
	HTTP_STATUS_UNSUPP_MEDIA_TYPE	= 415,
	HTTP_STATUS_REQ_RANGE_NOT_SATIS	= 416,
	HTTP_STATUS_EXPECTATION_FAILED	= 417,

	HTTP_STATUS_INTERNAL_SERVER_ERR	= 500,
	HTTP_STATUS_NOT_IMPLEMENTED		= 501,
	HTTP_STATUS_BAD_GATEWAY			= 502,
	HTTP_STATUS_SERVICE_UNAVAILABLE	= 503,
	HTTP_STATUS_GATEWAY_TIMEOUT		= 504,
	HTTP_STATUS_HTTP_VER_NOT_SUPP	= 505,
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * General purpose linked list. Actually isn't very portable because
 * uses only 'name' and 'value' variables to store data. Probably, in
 * a future release, this will be replaced by another type of struct.
 */
typedef struct formvarsA {
	char *name;
	char *value;
	struct formvarsA *next;
} formvars;

#ifdef __cplusplus
}
#endif

#endif /* CGI_TYPES_H */

/* vim: set noet sts=0 ts=4 sw=4 sr: */
