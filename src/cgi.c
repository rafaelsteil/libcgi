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

// There's no reason to not have this initialised.
static const char hextable[256] = {
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0X01, 0X02, 0X03,		0X04, 0X05, 0X06, 0X07,
	0X08, 0X09, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0X0A, 0X0B, 0X0C,		0X0D, 0X0E, 0X0F, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0X0A, 0X0B, 0X0C,		0X0D, 0X0E, 0X0F, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF
	};



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
	formvars *ret = NULL;
	char *method = getenv("REQUEST_METHOD");

	/* When METHOD has no contents, the default action is to process it as
	 * GET method
	 */
	if (! method || ! strcasecmp("GET", method))
	{
		char *q = getenv("QUERY_STRING");

		// Sometimes, GET comes without any data
		if (q && *q)
			ret = process_data(q, &formvars_start, &formvars_last, '=', '&');
	}
	else if (! strcasecmp("POST", method))
	{
		char *post_data;
		char *length_str;
		char *trailing;
		unsigned long length;
		/* no current support for file uploads, so limit upload data size
		 * 1 MB should be overkill for text data
		 */
		const unsigned long content_max = 1024 * 1024;

		length_str = getenv("CONTENT_LENGTH");
		if (! length_str || ! *length_str)
			return NULL;

		/* validate length. not checking for negative as is cast unsigned. */
		length = strtoul(length_str, &trailing, 10);
		if (*trailing != '\0' || ! length || length > content_max)
			return NULL;

		post_data = (char *)malloc(length + 1);
		if (! post_data)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		if (fread(post_data, sizeof(char), length, stdin) == length)
		{
			post_data[length] = '\0';
			ret = process_data(post_data, &formvars_start, &formvars_last,
			                   '=', '&');
		}

		free(post_data);
	}

	return ret;
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

/**
*  Main cgi function.
*  Configures all (most?) we need to  get cgi library working correctly. It MUST be called before
*  any other cgi function.
*  @see cgi_end, cgi_process_form, cgi_init_headers
**/
int cgi_init()
{
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
char *cgi_unescape_special_chars(const char *str)
{
	char *new, *write;
	char c = *str;
	char hex[2];

	new = (char *)malloc(strlen(str) + 1);
	if (! new)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (write = new; c; ++str, ++write)
	{
		c = *str;

		if (c == '+')
			c = ' ';
		else if (c == '%')
		{
			/* check first expected hex character isn't null to avoid reading
			 * second hex character from beyond the string
			 */
			if (str[1])
			{
				hex[0] = hextable[str[1]];
				hex[1] = hextable[str[2]];

				/* valid hex characters? */
				if (hex[0] != 0xFF && hex[1] != 0xFF)
				{
					c = (hex[0] << 4) | hex[1];
					str += 2;
				}
			}
		}

		*write = c;
	}

	return new;
}

/**
* Transforms' special characters into hexadecimal form ( %%E1 ).
* @param str String to parse
* @return The new string
* @see cgi_unescape_special_chars
**/
char *cgi_escape_special_chars(const unsigned char *str)
{
	static const unsigned char hex[] = "0123456789ABCDEF";
	int i, len;
	unsigned char *new;

	len = strlen(str);

	// worst case scenario: every character would need to be escaped, requiring
	// 3 times more memory than the original string.
	new = (unsigned char*)malloc((len * 3) + 1);
	if (! new)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (i = 0; *str; i++, str++)
	{
		if (*str == ' ')
			new[i] = '+';
		else if (! isalnum(*str) && ! strchr("_-.", *str))
		{
			new[i++] = '%';
			new[i++] = hex[*str >> 4];
			new[i]   = hex[*str & 0x0F];
		}
		else
			new[i] = *str;
	}

	new[i] = '\0';

	// free unused memory. no reason to fail.
	new = realloc(new, i+1);

	return new;
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
