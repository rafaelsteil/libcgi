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

#include "error.h"
#include "cgi.h"

formvars *cookies_start = NULL;
formvars *cookies_last = NULL;

extern int headers_initialized;
extern int cgi_display_errors;


/***********************************************************
				COOKIE GROUP
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
**/
int cgi_add_cookie(const char *name, 
	const char *value, 
	const char *max_age,
	const char *path, 
	const char *domain, 
	const int secure)
{
	if (headers_initialized)
		return 0;

	printf("Set-cookie: %s=%s;", name, value);

	if (max_age)
		printf(" Max-age=%s;", max_age);
	if (path)
		printf(" Path=%s;", path);
	if (domain)
		printf(" Domain=%s;", domain);
	if (secure)
		printf(" Secure");
	printf("\r\n");

	return 1;
}

formvars *cgi_get_cookies()
{
	register formvars *data;	
	register size_t position;
	char *cookies, *aux;

	if ((cookies = getenv("HTTP_COOKIE")) == NULL)
		return NULL;

	cookies = cgi_unescape_special_chars(cookies);
	aux = cookies;

	while (cookies) {
		position = 0;
		data = (formvars *)malloc(sizeof(formvars));
		if (!data)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		while (*aux++ != '=')
			position++;

		data->name = (char *)malloc(position+1);
		if (!data->name) {
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

			exit(EXIT_FAILURE);
		}

		strncpy(data->name, cookies, position);
		data->name[position] = '\0';

		position = 0;
		cookies = aux;
		if ((strchr(cookies, ';')) == NULL) {
			aux = NULL;
			position = strlen(cookies);
		}
		else {
			while (*aux++ != ';') 
				position++;
			// Eliminate the blank space after ';'
			aux++;
		}

		data->value = (char *)malloc(position + 1);
		if (!data->value) {
			exit(-1);
		}

		strncpy(data->value, cookies, position);
		data->value[position] = '\0';

		slist_add(data, &cookies_start, &cookies_last);
		cookies = aux;
	}

	return cookies_start;
}

/**
* Gets cookie value.
* Like cgi_param(), cgi_cookie_value() returns the data contained in cookie_name cookie
* @param cookie_name Cookie name to get the value
* @return The cookie value
**/
char *cgi_cookie_value(const char *cookie_name)
{
	return slist_item(cookie_name, cookies_start);
}

/**
* @}
*/

