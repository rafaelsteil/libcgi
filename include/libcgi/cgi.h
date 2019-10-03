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

/*****************************************************
 *					CGI GROUP
 *****************************************************/

/** @defgroup libcgi_cgi CGI manipulation
 * @{
 */

/**
 * Process HTML form or URL data.
 * Used to retrieve GET or POST data. It handles automatically the correct REQUEST_METHOD, so you don't need to afraid about it.
 * @return Returns the contents of URL or FORM into a formvars variable, or NULL if FALSE. Most of time, you
 * don't need any variable to store the form data, because is used an internal variable to manipulate the contents.
 * @see cgi_init, cgi_init_headers
 */
extern formvars *cgi_process_form(void);

/**
 * Kills the application with a message.
 * Writes msg and terminate
 * @param msg Message to send to the browser before killing
 */
extern void cgi_fatal(const char *error);

/**
 * Include static files.
 * Function used to include static data ( normally html files ).
 * File contents will never be processed.
 * Note that I don't scan for any special character. The reason
 * I did it is, if the you are using this library, you have a shell
 * where you can compile the cgi program. And can do much more ;-)
 * @param filename Filename with full path to include
 * @return If an error occurs and libcgi_debug is true, then a warning message is shown.
 * @see libcgi_debug
 *
 * \code
 * cgi_include("top_bar.htm");
 * \endcode
 */
extern int cgi_include(const char *path);

/**
 * Initialize HTML headers.
 * You need to call this function before any content is send to the browser, otherwise you'll get an error (Error 500).
 * @see cgi_init
 */
extern void cgi_init_headers(void);

/**
 * Return all values with the same name sent by a form.
 * @param name Form variable name
 * @return Form variable contents
 * @see cgi_param
 *
 * Example:
 * For example, if in your HTML you have something like
 * <br>
 * <br>
 * <pre>
 * "What do you like??"<br>
 * Computers : &lt;input type='checkbox' name='like' value='computers'&gt;&lt;br&gt;
 * Internet : &lt;input type='checkbox' name='like' value='net'&gt;&lt;br&gt;
 * games : &lt;input type='checkbox' name='like' 'value='games''&gt;&lt;br&gt;
 * </pre>
 * <br>
 * then, to retrieve all values, you can make a code like<br><br>
 *
 * \code
 * // ...
 * char *data;
 * // ...
 * while ((data = cgi_param_multiple("like")) != NULL)
 *     puts(data);
 * // ...
 * \endcode
 */
extern char *cgi_param_multiple(const char *name);

/**
 * Redirects to the specified url.
 * Remember that you cannot send any header before this function, or it will not work.
 * <b>Note:</b><br>
 * LibCGI does not implement RFC 2396 to make the lib simple and quick. You should be sure
 * to pass a correct URI to this function.
 * @param url url to redirect the browser to
 *
 * \code
 * cgi_redirect("http://wwww.linux.org");
 * \endcode
 */
CGI_DEPRECATED void cgi_redirect(char *url);

/**
 * Main cgi function.
 * Configures all (most?) we need to  get cgi library working correctly. It MUST be called before
 * any other cgi function.
 * @see cgi_end, cgi_process_form, cgi_init_headers
 */
extern int cgi_init(void);

/**
 * Performs cgi clean ups.
 * Provides some methods to clean memory or any other job that need to be done before the end of the application.
 * @see cgi_init
 */
extern void cgi_end(void);

/**
 * Transforms URL special chars.
 * Search for special chars ( like %%E1 ) in str, converting them to the ascii character correspondent.
 * @param str String containing data to parse
 * @return The new string
 * @see cgi_escape_special_chars
 */
extern char *cgi_unescape_special_chars(const char *str);

/**
 * Transforms special characters into hexadecimal form ( %%E1 ).
 * @param str String to parse
 * @return The new string
 * @see cgi_unescape_special_chars
 */
extern char *cgi_escape_special_chars(const char *str);

/**
 * Gets the of HTML or URL variable indicated by 'name'
 * @param name Form Variable name
 * @see cgi_param_multiple,  cgi_process_form, cgi_init
 *
 * \code
 * // ...
 * char *contents;
 *
 * cgi_init();
 * cgi_process_form();
 * cgi_init_headers();
 *
 * contents = cgi_param("foo");
 *
 * puts(contents);
 *
 * // ...
 * \endcode
 */
extern char *cgi_param(const char *var_name);

/**
 * Sends a specific header.
 * Sends a specific HTTP header. You won't need to add '\\n\\n' chars.
 * @param header HTTP header to send, without new line characters
 * @return True
 * @see cgi_init_headers
 */
extern void cgi_send_header(const char *header);

/**
 * The version of this library.
 *
 * @return Version string.
 */
const char *cgi_version( void );

/**
 * Send a HTTP header 'Location:' with the uri provided together with a
 * pseudo header 'Status:' with some HTTP status code. See RFC 3875,
 * section 6.3 (Response Header Fields) for details on how this is
 * supposed to work.
 *
 * @see https://tools.ietf.org/html/rfc3875#section-6.3
 *
 * @param[in] status_code HTTP status code number, pick it from our enum type
 * @param[in] uri URI to be sent with the Location: header
 */
void cgi_redirect_status( enum cgi_http_status_code status_code, const char *uri );

/**
* @}
*/

/**************************************************************
 *						GENERAL GROUP
 **************************************************************/

/** @defgroup libcgi_general General purpose
 * @{
 */

/**
 * Transforms HTML special chars.
 * Transforms chars like '<', '>' and others in HTML form,
 * like '&lt' and '&gt'
 * @param str String containing code to parse
 * @return The new string
 * @author Robert Csok <rcsok@gmx.de>
 */
extern char *htmlentities(const char *str);

/**
 * Converts a string to MD5 format.
 *
 * @param str String to convert to MD5 hash
 * @return MD5 hash code
 */
CGI_DEPRECATED char *md5(const char *str);

/**
* @}
*/

/***********************************************************
 *				COOKIE GROUP
 ***********************************************************/

/** @defgroup libcgi_cookie Cookies
 * @{
 */

/**
 * Send a cookie to the client.
 * @param name Cookie name
 * @param value Cookie value
 * @param max_age  Cookie time life, in seconds. A value equal to 0 ( zero ) means to discard the cookie when the session is done.
 * @param path Cookie path at the server
 * @param domain Domain where cookie will work :)
 * @param secure Secure or not
 * @see cgi_cookie_value
 *
 * \code
 * cgi_add_cookie("mycookie", "mycookie value", 0, 0, 0, 0);
 * \endcode
 */
extern int cgi_add_cookie(const char *name, const char *value, const char *max_age, const char *path, const char *domain, const int secure);

/**
 * ToDo: Add documentation
 * @return
 */
extern formvars *cgi_get_cookies(void);

/**
 * Gets cookie value.
 * Like cgi_param(), cgi_cookie_value() returns the data contained in cookie_name cookie
 * @param cookie_name Cookie name to get the value
 * @return The cookie value
 */
extern char *cgi_cookie_value(const char *cookie_name);

/**
* @}
*/

/*********************************************************
 * 					STRING GROUP
 *********************************************************/

/**
 * @defgroup libcgi_string Strings
 * General string manipulation and utilities functions
 * @{
 */

/**
 * Same to addslashes(), except that this one only do the action while 'n' is great than 0.
 * @param s String to parse
 * @param n Number of characters to work with.
 * @see addslashes()
 *
 * \code
 * char *name = "My test string is called \"foobar\"";
 * puts(name); // will display My test string is called "foobar"
 *
 * name = addnslashes(name, 31);
 * puts(name); // will display My test string is called \"foobar"
 * \endcode
 */
extern char *addnslashes(char *s, int n);

/**
 * Add slashes to a string when necessary.
 * Adds a '\' in every quote ( " ), apostrophe ( ' ) or  backslash ( \ )
 * It's useful when working with databases, for example, because
 * someone can try insert this characters to try hack the application...
 * @param *s String to parse
 * @return The new string, with slashes
 * @see stripslashes, addnslashes
 *
 * \code
 * char *name = "My test string is called \"foobar\"";
 * puts(name); // will display My test string is called "foobar"
 *
 * name = addslashes(name);
 * puts(name); // will display My test string is called \"foobar\"
 * \endcode
 */
extern char *addslashes(char *str);

/**
 * Strip no more than 'n' slashes from a string.
 * Strip the backslash character ( \ ) from a string, stopping after 'n' char
 * @param s String to parse
 * @param n Maximum number of chars to parse
 * @return The new string, without slashes
 * @see addslashes, stripslashes
 *
 * \code
 * char *name = "My another string is called \\\"blablabla\\\"";
 * puts(name); // will display My another string is called \"blablabla\"
 * name = stripslashes(name, 33);
 * puts(name); // will display My another string is called "blablabla\"
 * \endcode
 */
extern char *stripnslashes(char *s, int n);

/**
 * Strip slashes from a string.
 * Strip the backslash character ( \ ) from a string
 * @param s String to parse
 * @return The new string, without slashes
 * @see addslashes, stripnslashes
 *
 * \code
 * char *name = "My another string is called \\\"blablabla\\\"";
 * puts(name); // will display My another string is called \"blablabla\"
 * name = stripslashes(name);
 * puts(name); // will display My another string is called "blablabla"
 * \endcode
 */
extern char *stripslashes(char *str);

/**
 * Strip left white spaces from a string.
 * @param str String to parse
 * @return The new string, without left spaces
 * @author Original code was contribuition by Erik Jansson
 * @see rtrim,  trim
 *
 * \code
 * char *s = "     String with spaces    ";
 * printf("_%s_\n", s);
 * s = ltrim(s);
 * printf("_%s_\n", s);
 * \endcode
 */
extern char *cgi_ltrim(char *str);

/**
 * Strip right white spaces from a string.
 * @param str String to parse
 * @return The new string, without left spaces
 * @author Original code was contribuition by Erik Jansson
 * @see ltrim,  trim
 *
 * \code
 * char *s = "     String with spaces    ";
 * printf("_%s_\n", s);
 * s = rtrim(s);
 * printf("_%s_\n", s);
 * \endcode
 */
extern char *cgi_rtrim(char *str);

/**
 * Strip both left and right white spaces from a string.
 * @param str String to parse
 * @return The new string, without left spaces
 * @author Original code was contribuition by Erik Jansson
 * @see ltrim,  trim
 *
 * \code
 * char *s = "     String with spaces    ";
 * printf("_%s_\n", s);
 * s = trim(s);
 * printf("_%s_\n", s);
 * \endcode
 */
extern char *cgi_trim(char *str);

/**
 * Copy part of a string.
 * Copy  count characters from src, starting from start
 * @param src String to copy from
 * @param start Initial offset
 * @param count Number of chars to copy
 * @return The new string
 *
 * \code
 * char *part, *str = "Test one, test two";
 * part = substr(str, 1, 5);
 * puts(part); // -> est o
 * \endcode
 */
extern char *substr(char *src, const int start, const int count);

/**
 * Create an array from a string separated by some special char.
 * Divides the src string in pieces, each delimited by token
 * and storing the total of pieces in total
 * @param src String to parse
 * @param token Character delimiter to search.
 * @param total An integer variable passed as reference, which stores the total of
 * itens of the array
 * @return The array, where each item is one separated by token
 *
 * \code
 * char **pieces;
 * char *name = "This,is,a,string,of,test";
 * int total, i;
 * pieces = explode(name, ",", &total);
 * for (i = 0; i < total; i++)
 *  	  printf("Piece %d: %s\n", i, *(pieces+i));
 * \endcode
 */
extern char **explode(char *src, const char *token, int *total);

/**
 * Replace characters in a string, but not more than 'n'.
 * Replace all occourences of *delim on *src with characters pointed by *with,
 * stopping after 'n' char.
 * @param *src String to parse
 * @param *delim Character to search that will be replaced
 * @param with String to replace with
 * @param n Maximum number of chars to parse
 * @return The new string
 * @see str_replace
 *
 * \code
 * char *linux = "Linux C";
 * linux = str_nreplace(linux, "C", "Cool", strlen(linux));
 * puts(linux);
 * //  -- OR --
 * char *name = "rAfAel steil";
 * name = str_nreplace(name, "A", "a", 3);
 * puts(name);
 * \endcode
 */
extern char *str_nreplace(char *str, const char *delim, const char *with, int n);

/**
 * Replace characters in a string.
 * Replace all occourences of *delim on *src with characters pointed by *with.
 * The problem with the following code is that the function only searches for the
 * first character of *delim, ignoring the rest. Other problem is speed relacioned:
 * note that the function ever compare the length of *with to do the correct action.
 * @param src String to parse
 * @param delim Character to search that will be replaced
 * @param with String to replace with
 * @return The new string
 * @see str_nreplace
 *
 * \code
 * char *linux = "Linux C";
 * linux = str_replace(linux, "C", "Cool");
 * puts(linux);
 * //  -- OR --
 * char *name = "rAfAel steil";
 * name = str_replace(name, "A", "a");
 * puts(name);
 * \endcode
 */
extern char *str_replace(char *str, const char *delim, const char *with);

/**
 * Returns the position of a character in a string, but parses no more that 'n' chars
 * @param s String where the search will be done
 * @param ch Character to search
 * @param count Maximum number of chars to parse before exiting the function
 * @see strpos()
 */
extern int strnpos(char *s, char *ch, unsigned int count);

/**
 * Returns the position of a character in a string.
 * @param s String where the search will be done
 * @param ch Character to search
 * @param count Maximum number of ch to search
 * @see strnpos()
 */
extern int strpos(char *s, char *ch);

/**
 * Delete characters from a string.
 * Delete count characters of s, starting in start
 * @param s String to search
 * @param start Initial offset to begin search
 * @param count Number of characters to delete
 * @return The new string
 * @see strndel()
 *
 * \code
 * char *txt = "Some text to test anything";
 * puts(txt);
 * txt = strdel(txt, 2, 8);
 * puts(txt);
 * \endcode
 */
extern char *strdel(char *s, int start, int count);

/**
 * Reads an entire line.
 * Reads a line from the file specified by the file pointer passed
 * as parameter. This function is instead to replace the non-portable
 * GNU getline() function.
 *
 * @param s File pointer to the file to read from.
 * @return String containing the line read or NULL if no more line are available
 * @author Robert Csok
 */
extern char *recvline(FILE *fp);

/**
 * Makes a string.
 * Works like printf(), with the difference
 * that it returns a string that is the
 * concatenation of the values passed as parameter.
 *
 * @param *s Inicial String and optionally formatation parameters ( just %s is allowed )
 * @return The new String
 * \code
 * char *sql = make_string("INSERT INTO myTable VALUES ('%s', '%s', '%s')", varValue1, varValue2, varValue3);
 * \endcode
 * \todo String limits/error checking
 */
extern char *make_string(char *s, ...);

/**
 * Encodes a given string to its base64 form.
 *
 * @param *str String to convert
 * @return Base64 encoded String
 * @see str_base64_decode
 */
extern char *str_base64_encode(char *str);

/**
 * Decode a base64 encoded string.
 *
 * @param *str Encoded String to decode
 * @return The decoded string
 * @see str_base64_encode
 */
extern char *str_base64_decode(char *str);

/**
* @}
*/

/*****************************************************
 *					LIST GROUP
 *****************************************************/

/** @defgroup libcgi_list List manipulation
 * @{
 */

/**
 * Add a new item to the list.
 *
 * @param item Item to be added.
 * @param start ToDo
 * @param last ToDo
 */
extern void slist_add(formvars *item, formvars **start, formvars **last);

/**
 * Delete from list the item pointed by name.
 *
 * @param name Name of the item to be deleted.
 * @param start ToDo
 * @param last ToDo
 * @return ToDo
 */
extern int slist_delete(char *name, formvars **start, formvars **last);

/**
 * Returns the value of the item pointed by name.
 *
 * @param name Name of the item of which the value should be returned.
 * @param start ToDo
 * @return ToDo
 */
extern char *slist_item(const char *name, formvars *start);

/**
 * Free linked list allocated memory.
 *
 * @param start ToDo
 */
extern void slist_free(formvars **start);

/**
* @}
*/

/**
 * @defgroup libcgi_session Session Handling
 *
 * One of the most diferencials of LibCGI is its support to sessions, a mechanism that permits
 * the application to keep variables trough the user's session, when he is visiting your website.
 * \htmlonly
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

// We can use this variable to get the error message from a ( possible ) session error
// Use it togheter with session_lasterror
// i.e: printf("Session error: %s<br>", session_error_message[session_last_error]);
extern const char *session_error_message[];
extern int session_lasterror;

extern formvars *sess_list_start;

extern char SESSION_SAVE_PATH[255];
extern char SESSION_COOKIE_NAME[50];

/**
 * Destroys the session.
 * Destroys the current opened session, including all data.
 *
 * @note After session_destroy() was called, it is no more possible
 * to use session functions before another call to
 * session_start() …
 *
 * @see cgi_session_start()
 * @see cgi_session_error_message()
 *
 * @return True in case of success, false in case of errors.
 */
extern int cgi_session_destroy();

/**
 * Gets session variable's value.
 * @param name Session variable name to get the value
 * @return Variable contents if found, NULL if not.
 * @see cgi_session_var_exists()
 */
extern char *cgi_session_var(const char *name);

/**
 * Defines the name of the cookie that LibCGI will use to store session's ID.
 * This function works like cgi_session_save_path().
 * This functionality let you to use  different names for each site, but remember, you cannot
 * use multiple session for the same application yet.
 *
 * @param cookie_name Name of the cookie to create
 * @see cgi_session_save_path()
 * @note This function must be called before cgi_session_start()
 */
extern void cgi_session_cookie_name(const char *cookie_name);

/**
 * Defines where session control files will be saved.
 * If in the your CGI you don't make a call to cgi_session_save_path(), LibCGI will
 * use the default value, which is "/tmp/". To see how to modify the value, see the following example.
 * <br>Just note you need to add '/' at the end of the directory name
 *
 * \code
 * // your_cgi.c
 * // Set "session_files" directory under your CGI directory as the path
 * // which LibCGI will use to store session files.
 * cgi_session_save_path("session_files/");
 * \endcode
 *
 * Note that using this form, LibCGI will search for "session_files" directory using relative path
 * to your cgi application. For example, if your CGI script is located at
 * /usr/local/httpd/web/your_name/cgi-bin/ directory, and you use the above declaration, the files for the session will be
 * stored at  /usr/local/httpd/web/your_name/cgi-bin/session_files directory.
 * Resuming, the path is relative to where your application resides. <br><br>And remember, LibCGI \b does \b not
 * create the directory for you.
 *
 * @param path Path, relative or absolute
 * @see cgi_session_cookie_name
 * @note This function must be called before cgi_session_start()
 */
extern void cgi_session_save_path(const char *path);

/**
 * Register a variable in the current opened session.
 *
 * @note We are opening and closing the session file every time this
 * function is called... ( I/O ^ 1000000 :-/ )
 *
 * @param[in] name Variable name
 * @param[in] value Variable value
 *
 * @see cgi_session_alter_var(), cgi_session_unregister_var()
 *
 * @return True in case of success, false on error.
 */
extern int cgi_session_register_var(const char *name, const char *value);

/**
 * Alter session variable value.
 *
 * Change session variable 'name' value to data pointer by 'new_value'.
 *
 * @param[in] name Session variable name to change
 * @param[in] new_value New session variable value
 *
 * @see cgi_session_register_var(), cgi_session_unregister_var()
 *
 * @return True in case of success, false on error.
 */
extern int cgi_session_alter_var(const char *name, const char *new_value);

/**
 * Searches for determined session variable.
 *
 * @see cgi_session_var()
 *
 * @param[in] name Session variable name to search
 *
 * @return 1 (true) if variable is registered, 0 (false) if not
 */
extern int cgi_session_var_exists(const char *name);

/**
 * Unregister some session variable.
 * @param name Session variable name to unregister
 * @see cgi_session_var_exists(), cgi_session_register_var()
 */
extern int cgi_session_unregister_var(char *name);

/**
 * Starts a new session.
 *
 * This function is responsible for starting and creating a new
 * session. It must be called before any other session function, and
 * every time before any HTML header has sent.
 *
 * @see session_destroy()
 *
 * @return True aka 1 in case of success, false aka 0 otherwise.
 */
extern int cgi_session_start();

/**
 * Free all remaining things explicitly or implicitly allocated by a
 * session.
 *
 * @note This is some kind of workaround, you still have to call
 * cgi_end(), but maybe not cgi_destroy() … :-/
 *
 * @see https://github.com/rafaelsteil/libcgi/issues/25
 */
void cgi_session_free( void );

/**
 * ToDo: Check where this is coming from as it has no implementation
 * @param seconds
 */
extern void cgi_session_set_max_idle_time(unsigned long seconds);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif // _CGI_H
