/*
 * LibCGI - A library to make CGI programs using C
 *
 * Copyright 2001,2002 Rafael Steil <rafael@insanecorp.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 * License-Filename: LICENSES/LGPL-2.1.txt
 */

#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libcgi/error.h"

char *addnslashes(char *s, int n)
{
	char *tmp;
	int len, j = 0;

	if (s == NULL)
		return NULL;

	len = strlen(s);
	tmp = (char *)malloc(len+1);
	if (!tmp)
		return NULL;

	while (*s) {
		if ((n-- > 0) && ((*s == '\"') || (*s == '\'') || (*s == '\\'))) {
			len++;
			tmp = (char *)realloc(tmp, len);
			if (tmp == NULL)
				return NULL;

			tmp[j++] = '\\';
			tmp[j++] = *s++;
		}
		else
			tmp[j++] = *s++;
	}
	tmp[j] = '\0';

	return tmp;
}

char *addslashes(char *s)
{
	return addnslashes(s, strlen(s));
}

char *stripnslashes(char *s, int n)
{
	int j = 0;
	char *tmp;

	if (s == NULL)
		return NULL;

	tmp = (char *)malloc(strlen(s)+1);
	if (tmp == NULL)
		return NULL;

	while (*s) {
		if ((n-- > 0) && (*s == '\\'))
			s++;
		tmp[j++] = *s++;
	}

	tmp[j] = '\0';

	return tmp;
}

char *stripslashes(char *str)
{
	return stripnslashes(str, strlen(str));
}

char *cgi_ltrim(char *str)
{
    char *r, *s;

	r = s = str;

    /* nothing to do if str is NULL or zero-length */
	if (str && *str)
	{
		/* find first non-space character */
		while (isspace(*s))
			s++;

		/* if any space characters were found, remove them by copying from
		 * beyond them to the beginning of the source string
		 */
		if (s > r)
			while ((*r++ = *s++));
	}

	return str;
}

char *cgi_rtrim(char *str)
{
	char *last;

	/* nothing to do if str is NULL or zero-length */
	if (str && *str)
	{
		last = strchr(str, 0) - 1;

		/* loop backwards through @str to find last non-space character */
		for (; last >= str; --last)
		{
			if (! isspace(*last))
				break;
		}
		/* terminate @str after the last non-space character, or first
		 * character if only spaces were found
		 */
		last[1] = 0;
	}

	return str;
}

char *cgi_trim(char *str)
{
	return cgi_ltrim(cgi_rtrim(str));
}

char *substr(char *src, const int start, const int count)
{
	char *tmp;
	tmp = (char *)malloc(count+1);
	if (tmp == NULL) {
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		return NULL;
	}

	strncpy(tmp, src+start, count);
	tmp[count] = '\0';

	return tmp;
}

char **explode(char *src, const char *token, int *total)
{
	char **str;
	register int i, j, count, item, start;
	int len;

	if (!src || !token) {
		*total = 0;

		return NULL;
	}

	count = item = start = 0;
	j = 1;
	len = strlen(src);

	// Scans the string to verify how many pieces we heave
	for (i = 0; i < len; i++) {
		if (src[i] == *token)
			count++;
	}

	// We don't have any piece to explode. Returning...
	if (count == 0) {
		*total = 0;

		return NULL;
	}

	// Allocate memory for the structure ( count lines )
	str = (char **)malloc(count * sizeof(char *));
	if (str == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	// Now we'll going to get each piece and store it in the structure
	for (i = 0; i < len; i++) {
		if (src[i] != *token)
			j++;
		else {
			// Found one. Now we need to allocate memory to store data
			str[item] = (char *)malloc(j-start);
			if (str[item] == NULL) {
				libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

				return NULL;
			}

			*(str+item) = substr(src, start, j-1);

			str[item][j-start-1] = '\0';
			item++;
			start = j++;
		}
	}

	// The last one
	*(str+count) = (char *)malloc(j);
	if (str[count] == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	str[count] = substr(src, start, j);
	str[count][j-start] = '\0';
	*total = ++count;

	return str;
}

char *str_nreplace(char *src, const char *delim, const char *with, int n)
{
	unsigned int w_len, i, n_len, counter;
	char *buf;

	// w_len -> width length
	w_len = strlen(with);
	n_len = strlen(src);
	counter = 0;

	buf = (char *)malloc(n_len+1);
	if (buf == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	while (*src) {
		if ((n-- > 0) && (*src == *delim)) {
			// If length of *with is greater than 1, then we need
			// to realocate memory before copying
			if (w_len > 1) {
				n_len += w_len;

				buf = (char *)realloc(buf, n_len);
				if (buf == NULL)
					libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

				// replace the character delimited by *delim with
				// the characters pointed by *with
				for (i = 0; i < w_len; i++)
					buf[counter++] = with[i];
			}
			// otherwise, only do a simple character substitution
			buf[counter++] = *with;
		}
		else
			buf[counter++] = *src;

		src++;
	}

	buf[counter] = '\0';

	return buf;
}

char *str_replace(char *str, const char *delim, const char *with)
{
	return str_nreplace(str, delim, with, strlen(str));
}

// Just for compatibility with older versions of LibCGI
char *replace(char *str, const char *delim, const char *with)
{
	return str_nreplace(str, delim, with, strlen(str));
}

int strnpos(char *s, char *ch, unsigned int count)
{
	register unsigned int pos = 0, i;

	for (i = 0; i <= count && *s; i++) {
		if (*s++ == *ch)
			return pos;

		pos++;
	}

	return -1;
}

int strpos(char *s, char *ch)
{
	return strnpos(s, ch, strlen(s));
}

char *strdel(char *s, int start, int count)
{
	register int i, len, contador = 0;
	register char *tmp;

	len = strlen(s);

	if ((count+start) > len)
		return NULL;

	tmp = (char *)malloc(len - count + 1);
	if (tmp == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	for (i = 0; i < len; i++) {
		if ((i >= start) && (i < (start+count)))
			s++;
		else
			tmp[contador++] = *s++;
	}

	tmp[contador] = '\0';

	return tmp;
}

char *recvline(FILE *s)
{
	int i = 0, ch;
	char *buf = 0;
	size_t siz = 0;

	for (; (ch = fgetc(s)) != EOF; i++) {
		if (i == siz)
			buf = realloc(buf, siz += BUFSIZ);

		buf[i] = ch;

		if (buf[i] == '\n') {
			buf[i] = '\0';

			if (i > 0 && buf[i-1] == '\r')
				buf[i-1] = '\0';

			return buf;
		}
	}

	if (i > 0) {
		if (i == siz) buf = realloc(buf, siz + 1);
			buf[i] = '\0';

		return buf;
	}

	return NULL;
}

char *make_string(char *s, ...)
{
	va_list ptr, bkp;
	unsigned int len;
	char *str_return, *a, *str;

	str = s;

	va_start(ptr, s);

	va_copy(bkp, ptr);
	len = strlen(s);

	while (*str) {
		if (*str == '%') {
			str++;

			switch (*str) {
				case 's':
					a = (char *)va_arg(ptr, char *);
					if (a)
						len += strlen(a);
					break;
			}
		}

		str++;
	}

	str_return = (char *)malloc((len + 1) * sizeof(char));
	if (!str_return)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	vsprintf(str_return, s, bkp);

	va_end(ptr);
	va_end(bkp);

	str_return[len] = '\0';

	return str_return;
}

char *strcat_ex(const char *str1, const char *str2)
{
	char *new_str;
	unsigned int len;

	if (!str1 || !str2)
		return NULL;

	len = strlen(str1) + strlen(str2);

	new_str = (char *)malloc((len + 1) * sizeof(char*));
	if (!new_str)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	sprintf(new_str, "%s%s", str1, str2);

	new_str[len] = '\0';

	return new_str;
}
