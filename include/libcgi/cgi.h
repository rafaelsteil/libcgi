/*
 * LibCGI - A library to make CGI programs using C
 *
 * Copyright 2001 Rafael Steil <rafael@insanecorp.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 * License-Filename: LICENSES/LGPL-2.1.txt
 */

#ifndef _CGI_H
#define _CGI_H	1

#include <stdio.h>

#include <libcgi/cgi_types.h>

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

#ifdef __cplusplus
extern "C" {
#endif

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
