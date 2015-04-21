/*
    LibCGI - A library to make CGI programs using C
    Copyright (C) 2001/2002 Rafael Steil

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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "cgi.h"
#include "error.h"

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
* \code
* char *name = "My test string is called \"foobar\"";
* puts(name); // will display My test string is called "foobar"
*
* name = addnslashes(name, 31);
* puts(name); // will display My test string is called \"foobar"
* \endcode
*/
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

/**
* Add slashes to a string when necessary.
* Adds a '\' in every quote ( " ), apostrophe ( ' ) or  backslash ( \ )
* It's useful when working with databases, for example, because
* someone can try insert this caracters to try hack the application...
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
char *addslashes(char *s)
{
	return addnslashes(s, strlen(s));
}

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
char *stripslashes(char *str)
{
	return stripnslashes(str, strlen(str));
}

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
char * cgi_ltrim(char *str)
{
    char *s = str;

    /* nothing to do if str is NULL or zero-length */
	if (str && *str)
	{
		/* find first non-space character */
		while (isspace(*s))
			s++;

		/* if any space characters were found, remove them by copying from
		 * beyond them to the source string
		 */
		if (s > str)
			strcpy(str, s);
	}

	return str;
}

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
char * cgi_rtrim(char *str)
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
char * cgi_trim(char *str)
{
	return cgi_ltrim(cgi_rtrim(str));
}

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
**/
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

/**
* Create an array from a string separated by some special char.
*  Divides the src string in pieces, each delimited by token
*  and storing the total of pieces in total
* @param src String to parse
* @param token Character delimiter to search.
* @param total An integer variable passed as reference, which stores the total of
* itens of the array
* @return The array, where each item is one separeted by token
*
* \code
*
*  char **pieces;
*  char *name = "This,is,a,string,of,test";
*  int total, i;
*  pieces = explode(name, ",", &total);
*  for (i = 0; i < total; i++)
*  	printf("Piece %d: %s\n", i, *(pieces+i));
* \endcode
**/
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

/**
* Replace characteres in a string, but not more than 'n'.
*  Replace all occourences of *delim on *src with characteres pointed by *with,
*  stopping after 'n' char.
*  @param *src String to parse
*  @param *delim Character to search that will be replaced
*  @param with String to replace with
*  @param n Maximum number of chars to parse
*  @return The new string
*  @see str_replace
*
*  \code
*  char *linux = "Linux C";
*  linux = str_nreplace(linux, "C", "Cool", strlen(linux));
*  puts(linux);
* //  -- OR --
*  char *name = "rAfAel steil";
*  name = str_nreplace(name, "A", "a", 3);
*  puts(name);
*  \endcode
**/
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
			// otherwise, only do a simple character subtitution
			buf[counter++] = *with;
		}
		else
			buf[counter++] = *src;

		src++;
	}

	buf[counter] = '\0';

	return buf;
}

/**
* Replace characteres in a string.
*  Replace all occourences of *delim on *src with characteres pointed by *with.
*  The problem with the folowing code is that the function only searches for the
*  first caracter of *delim, ingoring the rest. Other problem is speed relacioned:
*  note that the function ever compare the length of *with to do the correct action.
*  @param src String to parse
*  @param delim Character to search that will be replaced
*  @param with String to replace with
*  @return The new string
*  @see str_nreplace
*
*  \code
*  char *linux = "Linux C";
*  linux = str_replace(linux, "C", "Cool");
*  puts(linux);
* //  -- OR --
*  char *name = "rAfAel steil";
*  name = str_replace(name, "A", "a");
*  puts(name);
*  \endcode
**/
char *str_replace(char *str, const char *delim, const char *with)
{
	return str_nreplace(str, delim, with, strlen(str));
}

// Just for compatibility with older versions of LibCGI
char *replace(char *str, const char *delim, const char *with)
{
	return str_nreplace(str, delim, with, strlen(str));
}

/**
 * Returns the position of a character in a string, but parses no more that 'n' chars
 * @param s String where the search will be done
 * @param ch Character to search
 * @param count Maximum number of chars to parse before exiting the function
 * @see strpos()
 */
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

/**
* Returns the position of a character in a string.
* @param s String where the search will be done
* @param ch Character to search
* @param count Maximum number of ch to search
* @see strnpos()
**/
int strpos(char *s, char *ch)
{
	return strnpos(s, ch, strlen(s));
}

/**
* Delete characters from a string.
*  Delete count characters of s, starting in start
*  @param s String to search
*  @param start Initial offset to begin search
*  @param count Number of characteres to delete
*  @return The new string
*  @see strndel()
*
*  \code
*  *txt = "Some text to test anything";
*  puts(txt);
*  txt = strdel(txt, 2, 8);
*  puts(txt);
* \endcode
**/
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

/**
* Reads an entire line.
* Reads a line from the file specified by the file pointer passed
* as parameter. This function is intead to replace the non-portable
* GNU getline() function.
*
* @param s File pointer to the file to read from.
* @return String containing the line read or NULL if no more line are available
* @author Robert Csok
**/
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
**/
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

/**
* @}
*/
