/*
 * LibCGI - A library to make CGI programs using C
 *
 * Copyright 2001 Rafael Steil <rafael@insanecorp.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 * License-Filename: LICENSES/LGPL-2.1.txt
 */

#include "libcgi/cgi.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "libcgi/cgi_types.h"
#include "libcgi/config.h"
#include "libcgi/error.h"

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
formvars *process_data(const char *query, formvars **start, formvars **last,
	                   const char sep_value, const char sep_name)
{
	formvars *item;
	const char *equal, *amp;
	size_t name_len;
	size_t value_len;
	char *str_unesc;
	const char *query_end = strchr(query, '\0');

	if (query == NULL)
		return *start;

	for ( ; query < query_end; query = amp + 1)
	{
		/* find end of name and value pair (with or without value) */
		for (amp = query; (*amp != sep_name) && (*amp); ++amp);

		/* find name and value separator (end of name) */
		for (equal = query; (*equal != sep_value) && (equal < amp); ++equal);

		name_len = equal - query;
		value_len = (*equal == sep_value) ? amp - (equal + 1) : 0;

		/* name could be zero-length if '&' or '=' is the first character.
		 * value is of no use without name.
		 */
		if (! name_len)
			continue;

		/* allocate and initialise memory for new formvars */
		item = (formvars *)calloc(1, sizeof(formvars));
		if (! item)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		/* add name and value to new formvar item */
		item->name = strndup(query, name_len);
		if (! item->name)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		if (value_len)
		{
			item->value = strndup(equal + 1, value_len);
			if (! item->value)
				libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

			str_unesc = cgi_unescape_special_chars(item->value);
			free(item->value);
			item->value = str_unesc;
		}

		slist_add(item, start, last);
	}

	return *start;
}

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
		const unsigned long content_max = 1024UL * 1024UL;

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

void cgi_fatal(const char *msg)
{
	cgi_init_headers();

	puts(msg);

	cgi_end();
	exit(1);
}

int cgi_include(const char *path)
/* flow: path != NULL
 *       get file stats (for file size)
 *       allocate memory for file contents
 *       open file stream
 *       read whole file into memory - includes shouldn't be huge
 *       write to stdout
 *       return number of bytes written (0 == failure)
 */
{
	struct stat fstats;
	FILE *fp = NULL;
	char *fcontents = NULL;
	size_t nwritten = 0;

	if (! path)
		goto err_input;

	if (stat (path, &fstats) == -1)
		goto err_input;

	if (! (fcontents = malloc (fstats.st_size)))
		goto err_memory;

	if (! (fp = fopen (path, "r")))
		goto err_input;

	if (fread (fcontents, sizeof(char), fstats.st_size, fp) != fstats.st_size)
		goto err_input;

	nwritten = fwrite (fcontents, sizeof(char), fstats.st_size, stdout);
	if (nwritten != fstats.st_size)
		goto err_output;

cleanup:
	if (fp)
		fclose (fp);
	free (fcontents);

	return nwritten;

err_input:
	libcgi_error(E_WARNING, "%s: file error: %s", __FUNCTION__, path);
	goto cleanup;

err_output:
	libcgi_error(E_WARNING, "%s: written: %u of %u",
	             __FUNCTION__, nwritten, fstats.st_size);
	goto cleanup;

err_memory:
	/* exit(EXIT_FAILURE) */
	libcgi_error(E_MEMORY, "%s: %s", __FILE__, __FUNCTION__);
	goto cleanup; /* silence compiler warnings */
}

void cgi_init_headers()
{
	if (!headers_initialized) {
		printf("Content-type: text/html\r\n\r\n");

		headers_initialized = 1;
	}
}

char *cgi_param_multiple(const char *name)
{
	/* DFrostByte: caching of 'name' would allow beginning new name search
	* without having to fetch all occurrences of previous.
	*
	* taking a formvars * parameter would be more flexible. if return was also
	* formvars *, the function would work in the same way without any
	* need for static variables which may be problematic in a shared lib
	*/
	static formvars *iter = NULL;
	char *value;

	if (! iter)
		iter = formvars_start;

	for (value = NULL; iter; iter = iter->next)
	{
		/* DFrostByte: shouldn't really be case-insensitive */
		if (! strcasecmp(iter->name, name))
		{
			value = iter->value;
			iter = iter->next;
			break;
		}
	}
	/* both iter and value will be NULL if no match was found */
	return value;
}

void cgi_redirect(char *url)
{
	if (headers_initialized) {
		libcgi_error(E_WARNING, "<br><b>Cannot redirect. Headers already sent</b><br>");

		return;
	}

	printf("Location: %s\r\n\r\n", url);
}

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

void cgi_end()
{
	slist_free(&formvars_start);

	formvars_last = NULL;

	if (sess_list_start)
		slist_free(&sess_list_start);

	if (cookies_start)
		slist_free(&cookies_start);
}

char *cgi_unescape_special_chars(const char *str)
{
	char *new, *write;
	char c;
	char hex[2];

	if ( !str ) return NULL;

	new = (char *) calloc( strlen(str) + 1, 1 );
	if (! new)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (write = new; *str; ++str, ++write)
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
				hex[0] = hextable[(unsigned char)str[1]];
				hex[1] = hextable[(unsigned char)str[2]];

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
	*write = '\0';

	// free unused memory. no reason to fail.
	new = realloc(new, strlen(new) + 1);

	return new;
}

char *cgi_escape_special_chars(const char *str)
{
	static const char hex[] = "0123456789ABCDEF";
	char *new;
	int i;
	size_t len = strlen(str);

	// worst case scenario: every character would need to be escaped, requiring
	// 3 times more memory than the original string.
	new = (char*)malloc((len * 3) + 1);
	if (! new)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (i = 0; *str; i++, str++)
	{
		if (*str == ' ')
			new[i] = '+';
		else if (! isalnum(*str) && ! strchr("_-.", *str))
		{
			new[i++] = '%';
			new[i++] = hex[(unsigned char)*str >> 4];
			new[i]   = hex[(unsigned char)*str & 0x0F];
		}
		else
			new[i] = *str;
	}
	new[i] = '\0';

	// free unused memory. no reason to fail.
	new = realloc(new, i+1);

	return new;
}

char *cgi_param(const char *var_name)
{
	return slist_item(var_name, formvars_start);
}

void cgi_send_header(const char *header)
{
	printf("%s\r\n", header);
}

const char *cgi_version( void )
{
	return CGI_VERSION;
}

void cgi_redirect_status( enum cgi_http_status_code status_code,
		const char *uri )
{
	if ( headers_initialized )
	{
		libcgi_error( E_WARNING,
				"<br><b>Cannot redirect. Headers already sent</b><br>" );
		return;
	}

	printf( "Status: %i\r\n"
			"Location: %s\r\n"
			"\r\n",
			status_code, uri );
}
