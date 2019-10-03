/*
 * LibCGI - A library to make CGI programs using C
 *
 * Copyright 2001 Rafael Steil <rafael@insanecorp.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 * License-Filename: LICENSES/LGPL-2.1.txt
 */

/*****************************************************
 * The variables/functions beginning with sess_ are
 * for manipulation internly only. You can use
 * the session_ like functions/variables
 *****************************************************
*/

#include "libcgi/session.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "libcgi/cgi.h"
#include "libcgi/cgi_types.h"
#include "libcgi/error.h"

// session id length
#define SESS_ID_LEN 45

// File pointer to session file in the server
FILE *sess_file;

static char sess_id[SESS_ID_LEN + 1];
static char *sess_fname = NULL;
static unsigned int save_path_len;

char SESSION_SAVE_PATH[255] = "/tmp/";
char SESSION_COOKIE_NAME[50] = "CGISID";

bool sess_initialized = false;
int session_lasterror = 0;

// We can use this variable to get the error message from a ( possible ) session error
// Use it togheter with session_lasterror
// i.e: printf("Session error: %s<br>", session_error_message[session_last_error]);
const char *session_error_message[] = {
 	"Session not initialized",
 	"Session file not initialized",
 	"Headers already been sent",
 	"Session already started",
 	"Failed to create session file",
 	"Failed to remove session file",
 	"Failed to destroy the session",
 	"Failed to remove session value from list",
 	"Session variable already registered",
 	"Session variable not registered",
 	"Failed to open session file for manipulation",
 	"Invalid argument"
};


// cgi.c
extern int headers_initialized;
extern void libcgi_error(int error_code, const char *msg, ...);
extern formvars *process_data(const char *query, formvars **start, formvars **last,
                              const char sep_value, const char sep_name);

// Error types
typedef enum SESS_ERROR {
	SESS_NOT_INITIALIZED,
	SESS_FILE_NOT_INITIALIZED,
	SESS_HEADERS_SENT,
	SESS_STARTED,
	SESS_CREATE_FILE,
	SESS_DELETE_FILE,
	SESS_DESTROY,
	SESS_REMOVE_FROM_LIST,
	SESS_VAR_REGISTERED,
	SESS_VAR_NOT_REGISTERED,
	SESS_OPEN_FILE,
	SESS_EINVAL
} sess_error;

// This variables are used to control the linked list of all
// session objects. Most of time you don't need to use them
// directly
formvars *sess_list_start = NULL;
formvars *sess_list_last = NULL;

// Generate a session "unique" id
void sess_generate_id()
{
	static char table[] = "123456789abcdefghijlmnopqrstuvxzwyABCDEFGHIJLMOPQRSTUVXZYW";
	unsigned int len = strlen(table);
	register int i;

 	save_path_len = strlen(SESSION_SAVE_PATH) + strlen(SESSION_FILE_PREFIX);

	sess_fname = (char *)malloc(save_path_len + SESS_ID_LEN + 1);
	if (!sess_fname)
		libcgi_error(E_MEMORY, "File %s, line %s", __FILE__, __LINE__);

	for (i = 0; i < SESS_ID_LEN; i++)
		sess_id[i] = table[rand()%len];
	sess_id[SESS_ID_LEN] = '\0';

	snprintf(sess_fname, (SESS_ID_LEN + save_path_len + 1), "%s%s%s", SESSION_SAVE_PATH, SESSION_FILE_PREFIX, sess_id);
	sess_fname[SESS_ID_LEN + save_path_len] = '\0';
}

int sess_create_file()
{
	// timeval, gettimeofday are used togheter with srand() function
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	sess_generate_id();
	sess_file = fopen(sess_fname, "w");
	if (!sess_file) {
		session_lasterror = SESS_CREATE_FILE;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}

	// Changes file permission to 0600
	chmod(sess_fname, S_IRUSR|S_IWUSR);
	fclose(sess_file);

	return 1;
}

int cgi_session_destroy( void )
{
	// Remember: unlink() returns 0 if success :)
	if (!unlink(sess_fname)) {
		sess_initialized = false;
		slist_free(&sess_list_start);

		// hhhmmm..
		if (headers_initialized)
			libcgi_error(E_WARNING, "Headers already sent. session_destroy() can't fully unregister the session");
		else
			cgi_add_cookie(SESSION_COOKIE_NAME, "", 0, 0, 0, 0);

		return true;
	}
	else {
		session_lasterror = SESS_DESTROY;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return false;
	}
}

int sess_file_rewrite()
{
	formvars *data;

	// Rewrites all data to session file
	sess_file = fopen(sess_fname, "w");

	if (!sess_file) {
		session_lasterror = SESS_OPEN_FILE;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}

	data = sess_list_start;

	if (data != NULL) {
		fprintf(sess_file, "%s=%s", data->name, data->value);
		data = data->next;

		if (data) {
			while (data) {
				fprintf(sess_file, ";%s=%s", data->name, data->value);
				data = data->next;
			}
		}
	}

	fclose(sess_file);

	return 1;
}

char *cgi_session_var(const char *var_name)
{
	return slist_item(var_name, sess_list_start);
}

void cgi_session_cookie_name(const char *cookie_name)
{
	strncpy(SESSION_COOKIE_NAME, cookie_name, 49);
}

void cgi_session_save_path(const char *path)
{
	strncpy(SESSION_SAVE_PATH, path, 254);
}

int cgi_session_register_var(const char *name, const char *value)
{
	formvars *data;

	if (!name) {
		session_lasterror = SESS_EINVAL;
		return false;
	}

	if (!sess_initialized) {
		session_lasterror = SESS_NOT_INITIALIZED;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return false;
	}

	if (!cgi_session_var_exists(name)) {
		sess_file = fopen(sess_fname, "a");
		if (!sess_file) {
			session_lasterror = SESS_OPEN_FILE;

			libcgi_error(E_WARNING, session_error_message[session_lasterror]);

			return false;
		}

		data = (formvars *)malloc(sizeof(formvars));
		if (!data)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		data->name = (char *)malloc(strlen(name) + 1);
		if (!data->name)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		data->value = (char *)malloc(strlen(value) + 1);

		if (!data->value) {
			free(data->name);

			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);
		}

		strncpy(data->name, name, strlen(name));
		data->name[strlen(name)] = '\0';

		strncpy(data->value, value, strlen(value));
		data->value[strlen(value)] = '\0';

		if (!sess_list_last)
			fprintf(sess_file, "%s=%s", name, value);
		else
			fprintf(sess_file, ";%s=%s", name, value);

		slist_add(data, &sess_list_start, &sess_list_last);

		fclose(sess_file);
		return true;
	}

	session_lasterror = SESS_VAR_REGISTERED;

	return false;
}

int cgi_session_alter_var(const char *name, const char *new_value)
{
	register formvars *data;
	unsigned int value_len;

	if (!name || !new_value) {
		session_lasterror = SESS_EINVAL;
		return false;
	}

	data = sess_list_start;
	while (data) {
		if (!strcmp(data->name, name)) {
			value_len = strlen(new_value) + 1;

			if (value_len > strlen(data->value)) {
				data->value = realloc(data->value, value_len+1);
				if (!data->value)
					libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

			}

			strncpy(data->value, new_value, value_len);
			data->value[value_len] = '\0';

			sess_file_rewrite();

			return true;
		}

		data = data->next;
	}

	session_lasterror = SESS_VAR_NOT_REGISTERED;

	return false;
}

int cgi_session_var_exists(const char *name)
{
	if (!slist_item(name, sess_list_start)) {
		session_lasterror = SESS_VAR_NOT_REGISTERED;
		return false;
	}

	return true;
}

int cgi_session_unregister_var(char *name)
{
	if (!sess_initialized) {
		session_lasterror = SESS_NOT_INITIALIZED;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}

	if (!slist_delete(name, &sess_list_start, &sess_list_last)) {
		session_lasterror = SESS_REMOVE_FROM_LIST;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}

	if (!sess_file_rewrite())
		return 0;

	return 1;
}

int cgi_session_start()
{
	char *buf = NULL, *sid = NULL;
	struct stat st;
	FILE *fp;

	if (sess_initialized) {
		session_lasterror = SESS_STARTED;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return false;
	}

	if (headers_initialized) {
		session_lasterror = SESS_HEADERS_SENT;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return false;
	}

	// Get the session ID
	sid = cgi_cookie_value(SESSION_COOKIE_NAME);

	// If there isn't a session ID, we need to create one
	if (sid == NULL) {
		if (sess_create_file()) {
			cgi_add_cookie(SESSION_COOKIE_NAME, sess_id, 0, 0, 0, 0);

			sess_initialized = true;

			return true;
		}

		return false;
	}
	// Make sure the file exists
	else {
		save_path_len = strlen(SESSION_SAVE_PATH) + strlen(SESSION_FILE_PREFIX);

		sess_fname = (char *)malloc(save_path_len + SESS_ID_LEN + 1);
		if (!sess_fname)
			libcgi_error(E_MEMORY, "File %s, line %s", __FILE__, __LINE__);

		snprintf(sess_fname, (SESS_ID_LEN + save_path_len + 1), "%s%s%s", SESSION_SAVE_PATH, SESSION_FILE_PREFIX, sid);
		sess_fname[SESS_ID_LEN + save_path_len] = '\0';

		errno = 0;
		fp = fopen(sess_fname, "r");
		if (errno == ENOENT) {
			// The file doesn't exists. Create a new session
			if (sess_create_file()) {
				cgi_add_cookie(SESSION_COOKIE_NAME, sess_id, 0, 0, 0, 0);

				libcgi_error(E_WARNING, "Session Cookie exists, but file don't. A new one was created.");
				sess_initialized = true;

				return true;
			}

			return false;
		}
	}

	// Well, at this point we've the session ID
	strncpy(sess_id, sid, SESS_ID_LEN);
	sess_id[SESS_ID_LEN] = '\0';

	// Now we need to read all the file contents
	// This is a temporary solution, I'll try to
	// make a faster implementation
	stat(sess_fname, &st);
	buf = (char *)malloc(st.st_size + 2);
	if (!buf)
		libcgi_error(E_MEMORY, "File %s, line %s", __FILE__, __LINE__);

	fgets(buf, st.st_size+1, fp);

	if (buf != NULL && strlen(buf) > 1)
		process_data(buf, &sess_list_start, &sess_list_last, '=', ';');

	fclose(fp);
	sess_initialized = true;
	free(buf);

	return true;
}

void cgi_session_free( void )
{
	free( sess_fname );
	sess_fname = NULL;
	sess_initialized = false;
}
