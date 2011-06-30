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

#ifdef __cplusplus
extern "C" {
#endif


// general purpose linked list. Actualy isn't very portable
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
extern void cgi_redirect(char *url);
extern void cgi_fatal(const char *error);
extern char *cgi_unescape_special_chars(char *str);
extern char *cgi_escape_special_chars(char *str);
extern char *cgi_param_multiple(const char *name);
extern char *htmlentities(const char *str);
extern int cgi_include(const char *filename);
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
extern char *md5(const char *str);

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
 
#ifdef __cplusplus
}
#endif

#endif // _CGI_H
