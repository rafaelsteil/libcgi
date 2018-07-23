/*
    LibCGI - A library to make CGI programs using C
    Copyright (C) 2001 Rafael Steil

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    You can contact the author by e-mail: rafael@insanecorp.com
*/
#ifndef _CGI_H
#define _CGI_H	1

#include <stdio.h>

#if defined(__GNUC__)
#define CGI_DEPRECATED __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define CGI_DEPRECATED __declspec(deprecated)
#elif defined(__clang__)
#define CGI_DEPRECATED __deprecated
#else
#pragma message("WARNING: You need to implement CGI_DEPRECATED for this compiler")
#define CGI_DEPRECATED
#endif

/**
 *	HTTP status codes.
 *
 *	@see	http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
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

// general purpose linked list. Actually isn't very portable
// because uses only 'name' and 'value' variables to store data.
// Problably, in a future release, this will be replaced by
// another type of struct
typedef struct formvarsA {
        char *name;
        char *value;
        struct formvarsA *next;
} formvars;

extern formvars *formvars_start;
extern formvars *formvars_last;
extern formvars *cookies_start;
extern formvars *cookies_last;

extern int cgi_display_errors;

// General purpose cgi functions
extern void cgi_init_headers(void);
CGI_DEPRECATED void cgi_redirect(char *url);
extern void cgi_fatal(const char *error);
extern char *cgi_unescape_special_chars(const char *str);
extern char *cgi_escape_special_chars(const char *str);
extern char *cgi_param_multiple(const char *name);
extern char *htmlentities(const char *str);
extern int cgi_include(const char *path);
extern formvars *cgi_process_form(void);
extern int cgi_init(void);
extern void cgi_end(void);
extern char *cgi_param(const char *var_name);
extern void cgi_send_header(const char *header);

// Cookie functions
extern int cgi_add_cookie(const char *name, const char *value, const char *max_age, const char *path, const char *domain, const int secure);
extern formvars *cgi_get_cookies(void);
extern char *cgi_cookie_value(const char *cookie_name);

// General purpose string functions
extern int strnpos(char *s, char *ch, unsigned int count);
extern int strpos(char *s, char *ch);
extern char *strdel(char *s, int start, int count);
extern char **explode(char *src, const char *token, int *total);
extern char *substr(char *src, const int start, const int count);
extern char *stripnslashes(char *s, int n);
extern char *addnslashes(char *s, int n);
extern char *stripnslashes(char *s, int n);
extern char *str_nreplace(char *str, const char *delim, const char *with, int n);
extern char *str_replace(char *str, const char *delim, const char *with);
extern char *addslashes(char *str);
extern char *stripslashes(char *str);
extern char *str_base64_encode(char *str);
extern char *str_base64_decode(char *str);
extern char *recvline(FILE *fp);
CGI_DEPRECATED char *md5(const char *str);
extern char *cgi_ltrim(char *str);
extern char *cgi_rtrim(char *str);
extern char *cgi_trim(char *str);

extern void slist_add(formvars *item, formvars **start, formvars **last);
extern int slist_delete(char *name, formvars **start, formvars **last);
extern char *slist_item(const char *name, formvars *start);

extern void slist_free(formvars **start);

// Session stuff
// We can use this variable to get the error message from a ( possible ) session error
// Use it togheter with session_lasterror
// i.e: printf("Session error: %s<br>", session_error_message[session_last_error]);
extern const char *session_error_message[];
extern int session_lasterror;

extern formvars *sess_list_start;

extern char SESSION_SAVE_PATH[255];
extern char SESSION_COOKIE_NAME[50];

extern void cgi_session_set_max_idle_time(unsigned long seconds);
extern int cgi_session_destroy();
extern int cgi_session_register_var(const char *name, const char *value);
extern int cgi_session_alter_var(const char *name, const char *new_value);
extern int cgi_session_var_exists(const char *name);
extern int cgi_session_unregister_var(char *name);
extern int cgi_session_start();
extern void cgi_session_cookie_name(const char *cookie_name);
extern char *cgi_session_var(const char *name);
extern void cgi_session_save_path(const char *path);

/**
 *	Free all remaining things explicitly or implicitly allocated by a
 *	session.
 *
 *	@note	This is some kind of workaround, you still have to call
 *			cgi_end(), but maybe not cgi_destroy() … :-/
 *
 *	@see	https://github.com/rafaelsteil/libcgi/issues/25
 */
void cgi_session_free( void );

/**
 *	The version of this library.
 *
 *	@return	Version string.
 */
const char *cgi_version( void );

/**
 *	Send a HTTP header 'Location:' with the uri provided together with a
 *	pseudo header 'Status:' with some HTTP status code. See RFC 3875,
 *	section 6.3 (Response Header Fields) for details on how this is
 *	supposed to work.
 *
 *	@see	https://tools.ietf.org/html/rfc3875#section-6.3
 *
 *	@param[in]	status_code	HTTP status code number, pick it from our enum type
 *	@param[in]	uri			URI to be sent with the Location: header
 */
void cgi_redirect_status( enum cgi_http_status_code status_code,
		const char *uri );

#ifdef __cplusplus
}
#endif

#endif // _CGI_H
