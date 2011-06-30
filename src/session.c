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

/*****************************************************
 * The varibles/functions beginning with sess_ are
 * for manipulation internly only. You can use
 * the session_ like functions/variables
 *****************************************************
*/

/** 
* @defgroup libcgi_session Session Handling
*   One of the most diferencials of LibCGI is its support to sessions, a mechanism that permits
*   the application to keep variables trough the user's session, when he is visiting your website.
*   \htmlonly
* <table width='90%' align='center' border='0' bgcolor='#cccccc'>
* <tr>
* <td>
* 	Session functions are easy to use and understand, and probably you'll want to take a closer
* 	look at <i>cgi_session_save_path()</i> and <i>cgi_session_cookie_name()</i> functions. These functions, 
* 	let the programmer to set the directory where session files will
* 	be saved in the hard disk and the cookie name to the session, respectively. 
* 	<br><br>
* 	As the CGI is running on the webserver which a common user, it have full access to its respective session
* 	file. But the most big problem is that you may
* 	have full access to all other session files as well, even from other sites. Yes, that's a big problem, and still
* 	other large used scripting languages like PHP does have this kind of problem (using the default installation).
* 	So, LibCGI is trying to
* 	make a bit harder to a potential attacker to stole session data or even destroy it.
* 	Now is possible to store session files in different locations, specified
* 	by the programmer ( using <i>cgi_session_save_path()</i> function ), as described in this doc ). <br>
* 	And yes, I ( me, LibCGI's author ) knows that there are problems yet, so your opinion is very important.
* 	You will find some useful examples under "examples/sessions" directory.
* </td>
* </tr>
* </table>
* \endhtmlonly
* @{
*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "cgi.h"
#include "session.h"
#include "error.h"

// session id length
#define SESS_ID_LEN 45

// File pointer to session file in the server
FILE *sess_file;

static char sess_id[SESS_ID_LEN + 1];
static char *sess_fname;
static unsigned int save_path_len;

char SESSION_SAVE_PATH[255] = "/tmp/";
char SESSION_COOKIE_NAME[50] = "CGISID";

int sess_initialized = 0;; // true if the session has been initialized, otherwise false
int sess_finitialized = 0; // true if the session file has been created, otherwirte false
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
 	"Failed to open session file for manipulation"
};
 

// cgi.c
extern int headers_initialized;
extern void libcgi_error(int error_code, const char *msg, ...);
extern formvars *process_data(char *query, formvars **start, formvars **last, const char delim, const char sep);

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
	SESS_OPEN_FILE
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

/** 
* Destroys the session. 
* Destroys the current opened session, including all data.
* After session_destroy() was called, is not more
* possible to use session functions before an another
* call to session_start()
* @return 1 if no errors, 0 if.
* @see cgi_session_start
* @see cgi_session_error_message
*/
int cgi_session_destroy()
{
	// Remember: unlink() returns 0 if success :)
	if (!unlink(sess_fname)) {
		sess_finitialized = 0;
		slist_free(&sess_list_start);

		// hhhmmm.. 
		if (headers_initialized) 
			libcgi_error(E_WARNING, "Headers alreay sent. session_destroy() can't fully unregister the session");
		else 
			cgi_add_cookie(SESSION_COOKIE_NAME, "", 0, 0, 0, 0);

		return 1;
	}
	else {
		session_lasterror = SESS_DESTROY;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}
}

int sess_file_rewrite()
{
	formvars *data;
	
	cgi_init_headers();
	
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


/**
* Gets session variable's value.
* @param name Session variable name to get the value
* @return Variable contents if found, NULL if not.
* @see cgi_session_var_exists()
*/
char *cgi_session_var(const char *var_name)
{
	return slist_item(var_name, sess_list_start);
}

/**
* Defines the name of the cookie that LibCGI will use to store session's ID.
* This function works like cgi_session_save_path(). 
* This functionality let you to use  different names for each site, but remember, you cannot 
* use multiple session for the same application yet. 
* 
* @param cookie_name Name of the cookie to create
* @see cgi_session_save_path()
* @note This function must be called before cgi_session_start()
**/
void cgi_session_cookie_name(const char *cookie_name)
{
	strncpy(SESSION_COOKIE_NAME, cookie_name, 49);
}

/**
* Defines where session control files will be saved.
* If in the your CGI you don't make a call to cgi_session_save_path(), LibCGI will
* use the default value, which is "/tmp/". To see how to modify the value, see the following example. 
* <br>Just note you need to add '/' at the end of the directory name
* \code
* // your_cgi.c
* // Set "session_files" directory under your CGI directory as the path 
* // which LibCGI will use to store session files.
* 
* cgi_session_save_path("session_files/");
*  \endcode
*  
*  Note that using this form, LibCGI will search for "session_files" directory using relative path 
*  to your cgi application. For example, if your CGI script is located at
*  /usr/local/httpd/web/your_name/cgi-bin/ directory, and you use the above declaration, the files for the session will be
* stored at  /usr/local/httpd/web/your_name/cgi-bin/session_files directory. 
* Resuming, the path is relative to where your application resides. <br><br>And remember, LibCGI \b does \b not
* create the directory for you. 
* 
* @param path Path, relative or absolute
* @see cgi_session_cookie_name
* @note This function must be called before cgi_session_start()
**/
void cgi_session_save_path(const char *path)
{
	strncpy(SESSION_SAVE_PATH, path, 254);
}

/**
* Register a variable in the current opened session.
* Note that we are opening and closing the session file
* every time this function is called... ( I/O ^ 1000000 :-/ )
* @param name Variable name
* @param value Variable value
* @see cgi_session_alter_var(), cgi_session_unregister_var()
*/
int cgi_session_register_var(const char *name, const char *value)
{
	formvars *data;
	
	if (!sess_initialized) {
		session_lasterror = SESS_NOT_INITIALIZED;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);
		
		return 0;
	}

	if (!cgi_session_var_exists(name)) {
		sess_file = fopen(sess_fname, "a");
		if (!sess_file) {
			session_lasterror = SESS_OPEN_FILE;

			libcgi_error(E_WARNING, session_error_message[session_lasterror]);
			
			return 0;
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
		return 1;
	}

	session_lasterror = SESS_VAR_REGISTERED;

	return 0;
}

/**
* Alter session variable value.
* Change session variable 'name' value to data pointer by 'new_value'
* @param name Session variable name to change
* @param new_value New session variable value
* @see cgi_session_register_var(), cgi_session_unregister_var()
*/
int cgi_session_alter_var(const char *name, const char *new_value)
{
	register formvars *data;
	unsigned int value_len;
	
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

			return 1;
		}

		data = data->next;
	}
	
	session_lasterror = SESS_VAR_NOT_REGISTERED;
	
	return 0;
}

/**
* Searches for determined session variable.
* @param name Session variable name to search
* @return 1 if variable is registered, 0 if not
* @see cgi_session_var()
*/
int cgi_session_var_exists(const char *name)
{
	if (!slist_item(name, sess_list_start)) {
		session_lasterror = SESS_VAR_NOT_REGISTERED;
		return 0;
	}

	return 1;
}

/**
* Unregister some session variable.
* @param name Session variable name to unregister
* @see cgi_session_var_exists(), cgi_session_register_var()
*/
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

/**
* Starts a new session.
* This function is responsible for starting and creating a new 
* session. It must be called before any other session function,
* and every time before any HTML header has sent.
* @see session_destroy()
*/
int cgi_session_start()
{
	char *buf = NULL, *sid = NULL;
	struct stat st;
	FILE *fp;

	if (sess_initialized) {
		session_lasterror = SESS_STARTED;

		libcgi_error(E_WARNING, session_error_message[session_lasterror]);
		
		return 0;
	}
	
	if (headers_initialized) {
		session_lasterror = SESS_HEADERS_SENT;
		
		libcgi_error(E_WARNING, session_error_message[session_lasterror]);

		return 0;
	}

	// Get the session ID
	sid = cgi_cookie_value(SESSION_COOKIE_NAME);

	// If there isn't a session ID, we need to create one 
	if (sid == NULL) {
		if (sess_create_file()) {
			cgi_add_cookie(SESSION_COOKIE_NAME, sess_id, 0, 0, 0, 0);

			sess_initialized = 1;

			return 1;
		}

		return 0;
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
				sess_initialized = 1;

				return 1;
			}

			return 0;
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
	sess_initialized = 1;
	free(buf);

	return 1;
}

/** 
* @}
*/
