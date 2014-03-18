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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cgi.h"
#include "error.h"

static unsigned char hextable['f'+1];

int headers_initialized = 0;

formvars *formvars_start = NULL;
formvars *formvars_last = NULL;

// session.c
extern formvars *sess_list_start;

// Set to 1 to activate runtime debugation, 0 to disable it
int cgi_display_errors = 1;

// cookie.c
extern formvars *cookie_start;
extern formvars *cookie_end;

// Separates *query in name=value pairs, then gets each piece of result of them, storing
// the result in the linked list global variable
formvars *process_data(char *query, formvars **start, formvars **last, const char delim, const char sep)
{
	register size_t position = 0, total_len = 0, i = 0;
	char *aux, *str_unesc;
	formvars *data;

	if (query == NULL)
		return *start;

	total_len = strlen(query);
	aux = query;
	while (*query) {
		position = 0;

		data = (formvars *)malloc(sizeof(formvars));
		if (!data)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		memset(data, 0, sizeof(formvars));

		// Scans the string for the next 'delim' character
		while (*aux && (*aux != delim)) {
			position++;
			aux++;
			i++;
		}

		if (*aux) {
			aux++;
			i++;
		}

		data->name = (char *)malloc(position+1);
		if (data->name == NULL)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		strncpy(data->name, query, position);
		data->name[position] = '\0';

		query = aux;
		position = 0;
		while (*aux && (*aux != sep)) {
			if ((*aux == '%') && (i + 2 <= total_len)) {
				if (isalnum(aux[1]) && isalnum(aux[2])) {
					aux += 2;
					i += 2;
					position++;
				}
			}
			else
				position++;

			aux++;
			i++;
		}

		if (*aux) {
			aux++;
			i++;
		}

		data->value = (char *)malloc(position+1);
		if (data->value == NULL)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		str_unesc = cgi_unescape_special_chars(query);
		strncpy(data->value, str_unesc, position);
		data->value[position] = '\0';
		free(str_unesc);

		slist_add(data, start, last);

		query = aux;
	}

	return *start;
}

/*****************************************************
					CGI GROUP
*****************************************************/
/** @defgroup libcgi_cgi CGI manipulation
* @{
*/

/**
* Process HTML form or URL data.
* Used to retrieve GET or POST data. It handles automaticaly the correct REQUEST_METHOD, so you don't need to afraid about it.
* @return Returns the contents of URL or FORM into a formvars variable, or NULL if FALSE. Most of time, you
* don't need any variable to store the form data, because is used an internal variable to manipulate the contents.
* @see cgi_init, cgi_init_headers
**/
formvars *cgi_process_form()
{
	char *method;

	method = getenv("REQUEST_METHOD");

	// When METHOD has no contents, the default action is to process it as GET method
	if (method == NULL || !strcasecmp("GET", method)) {
		char *dados;
		dados =	getenv("QUERY_STRING");

		// Sometimes, GET comes with not any data
		if (dados == NULL || strlen(dados) < 1)
			return NULL;

		return process_data(dados, &formvars_start, &formvars_last, '=', '&');
	}
	else if (!strcasecmp("POST", method)) {
		char *post_data;
		char *tmp_data;
		int content_length;
		formvars *ret;

		tmp_data = getenv("CONTENT_LENGTH");
		if (tmp_data == NULL)
			return NULL;

		content_length = atoi(tmp_data);

		post_data = (char *)malloc(content_length + 1);
		if (post_data == NULL)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		fread(post_data, content_length, 1, stdin);
		post_data[content_length] = '\0';

		ret = process_data(post_data, &formvars_start, &formvars_last, '=', '&');
		free(post_data);
		return ret;
	}

	return NULL;
}

/**
* Kills the application with a message.
* Writes msg and terminate
* @param msg Message to send to the browser before killing
*/
void cgi_fatal(const char *msg)
{
	cgi_init_headers();

	puts(msg);

	cgi_end();
	exit(1);
}

/**
* Include static files.
* Function used to include static data ( normaly html files ).
* File contents will never be processed.
* Note that I don't scan for any special character. The reason
* I did it is, if the you are using this library, you have a shell
* where you can compile the cgi program. And can do much more ;-)
* @param filename Filename with full path to include
* @return If an error occurs and libcgi_debug is true, then a warning
* message is showed.
* @see libcgi_debug
*
* \code
* cgi_include("top_bar.htm");
* \endcode
*/
int cgi_include(const char *filename)
{
	FILE *inc;
	char buffer[255];

	if (!(inc = fopen(filename, "r"))) {
		cgi_init_headers();

		libcgi_error(E_WARNING, "Failed to open include file <b>%s</b>", filename);

		return 0;
	}

 	while (fgets(buffer, 255, inc))
		printf("%s", buffer);

	fclose(inc);

	return 1;
}

/**
* Initialize HTML headers.
* You need to call this function before that any content is send to the brosert, otherwise you'll get an error (Error 500).
* @see cgi_init
**/
void cgi_init_headers()
{
	if (!headers_initialized) {
		printf("Content-type: text/html\r\n\r\n");

		headers_initialized = 1;
	}
}

/**
* Return all values with the same name sent by a form.
* @param name Form variable name
* @return Form variable contents
* @see cgi_param
*
* Example:
* For example, if in your HTML you have something like<br>
*  <br>
* <pre>
* "What do you like??"<br>
*  Computers : &lt;input type='checkbox' name='like' value='computers'&gt;&lt;br&gt;
*  Internet : &lt;input type='checkbox' name='like' value='net'&gt;&lt;br&gt;
*  games : &lt;input type='checkbox' name='like' 'value='games''&gt;&lt;br&gt;
* </pre>
*       <br>
* then, to retrieve all values, you can make a code like<br><br>
*
* \code
* // ...
* char *data;
* \\ ...
* while ((data = cgi_param_multiple("like")) != NULL)
* 	puts(data);
* \\ ...
* \endcode
**/
char *cgi_param_multiple(const char *name)
{
	static unsigned int counter = 0;
	unsigned int pos = 0;
	formvars *begin;

	begin = formvars_start;

	while (begin) {
		if ((!strcasecmp(begin->name, name)) && (++pos > counter)) {
			counter++;

			return (begin->value);
		}

		begin = begin->next;
	}

	counter = 0;

	return NULL;
}

/**
*  Recirects to the specified url.
* Remember that you cannot send any header before this function, or it will not work.
* <b>Note:</b><br>
* LibCGI does not implement RFC 2396 to make the lib simple and quick. You should be sure
* to pass a correct URI to this function.
* @param url url to redirect the browser
*
* \code
* cgi_redirect("http://wwww.linux.org");
* \endcode
**/
void cgi_redirect(char *url)
{
	if (headers_initialized) {
		libcgi_error(E_WARNING, "<br><b>Cannot redirect. Headers already sent</b><br>");

		return;
	}

	printf("Location: %s\r\n\r\n", url);
}

// Original idea from cgic library
void init_hex_table()
{
	memset(hextable, 0, 'f'+1);

	hextable['0'] = 0x0;	/* 48 */
	hextable['1'] = 0x1;	/* 49 */
	hextable['2'] = 0x2;	/* 50 */
	hextable['3'] = 0x3;	/* 51 */
	hextable['4'] = 0x4;	/* 52 */
	hextable['5'] = 0x5;	/* 53 */
	hextable['6'] = 0x6;	/* 54 */
	hextable['7'] = 0x7;	/* 55 */
	hextable['8'] = 0x8;	/* 56 */
	hextable['9'] = 0x9;	/* 57 */

	hextable['A'] = 0xA;	/* 65 */
	hextable['B'] = 0xB;	/* 66 */
	hextable['C'] = 0xC;	/* 67 */
	hextable['D'] = 0xD;	/* 68 */
	hextable['E'] = 0xE;	/* 69 */
	hextable['F'] = 0xF;	/* 70 */

	hextable['a'] = 0xa;	/* 97 */
	hextable['b'] = 0xb;	/* 98 */
	hextable['c'] = 0xc;	/* 99 */
	hextable['d'] = 0xd;	/* 100 */
	hextable['e'] = 0xe;	/* 101 */
	hextable['f'] = 0xf;	/* 102 */
}

/**
*  Main cgi function.
*  Configures all (most?) we need to  get cgi library working correctly. It MUST be called before
*  any other cgi function.
*  @see cgi_end, cgi_process_form, cgi_init_headers
**/
int cgi_init()
{
	init_hex_table();

	// Well... the reason I put cgi_get_cookies() here is to not
	// cause problems with session's. Note that, when you want
	// to use session within your program, you need  cgi_get_cookies()
	// before session_start(), otherwise we will get some problems... :)
	// Calling this function here is the best way. Trust me :)
	cgi_get_cookies();

	return 1;
}

/**
* Performs cgi clean ups.
* Provides some methods to clean memory or any other job that need to be done before the end of the application.
* @see cgi_init
**/
void cgi_end()
{
	slist_free(&formvars_start);

	formvars_last = NULL;

	if (sess_list_start)
		slist_free(&sess_list_start);

	if (cookies_start)
		slist_free(&cookies_start);
}

/**
* Transforms' URL special chars.
* Search for special chars ( like %%E1 ) in str, converting them to the ascii character correspondent.
* @param str String containing data to parse
* @return The new string
* @see cgi_escape_special_chars
**/
char *cgi_unescape_special_chars(char *str)
{
	char *tmp;
	register int i, len, pos = 0;

	len = strlen(str);
	tmp = (char *)malloc(len + 1);
	if (tmp == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (i = 0; i < len; i++) {
		// If we found a '%' character, then the next two are the character
		// hexa code. Converting a hexadecimal code to their decimal is easy:
		// The first character needs to be multiplied by 16 ( << 4 ), and the another
		// one we just get the value from hextable variable
		if ((str[i] == '%') && isxdigit(str[i+1]) && isxdigit(str[i+2])) {
			tmp[pos] = (hextable[(unsigned char) str[i+1]] << 4) + hextable[(unsigned char) str[i+2]];
			i += 2;
		}
		else if (str[i] == '+')
			tmp[pos] = ' ';
		else
			tmp[pos] = str[i];

		pos++;
	}

	tmp[pos] = '\0';

	return tmp;
}

/**
* Transforms' special characters into hexadecimal form ( %%E1 ).
* @param str String to parse
* @return The new string
* @see cgi_unescape_special_chars
**/
char *cgi_escape_special_chars(char *str)
{
	unsigned char hex[] = "0123456789ABCDEF";
	register int i, j, len, tmp_len;
	unsigned char *tmp;

	len = strlen(str);
	tmp_len = len;
	tmp = (unsigned char*)malloc(len+1);
	if (tmp == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (i = 0, j = 0; i < len; i++, j++) {
		tmp[j] = (unsigned char)str[i];
		if (tmp[j] == ' ')
			tmp[j] = '+';
		else if (!isalnum(tmp[j]) && strchr("_-.", tmp[j]) == NULL) {
			tmp_len += 3;
			tmp = realloc(tmp, tmp_len);
			if (!tmp)
				libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

			tmp[j++] = '%';
			tmp[j++] = hex[(unsigned char)str[i] >> 4];
			tmp[j] = hex[(unsigned char)str[i] & 0x0F];
		}
	}

	tmp[j] = '\0';

	return tmp;
}

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
**/
char *cgi_param(const char *var_name)
{
	return slist_item(var_name, formvars_start);
}

/**
* Sends a specific header.
* Sends a specific HTTP header. You won't need to add '\\n\\n' chars.
* @param header HTTP header to send, without new line characteres
* @return True
* @see cgi_init_headers
**/
void cgi_send_header(const char *header)
{
	printf("%s\r\n", header);
}


/**
* @}
*/
