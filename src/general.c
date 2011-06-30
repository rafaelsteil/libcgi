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
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "cgi.h"

struct iso8859_15 {
	char code;
	char *html;
} he[] = {
	{ 0xE4, "&auml;" },
	{ 0xC4, "&Auml;" },
	{ 0xF6, "&ouml;" },
	{ 0xD6, "&Ouml;" },
	{ 0xFC, "&uuml;" },
	{ 0xDC, "&Uuml;" },
	{ 0xDF, "&szlig;" },
	{ 0xA4, "&euro;" },
	{ 0x3C, "&lt;"  },
	{ 0x3E, "&gt;"  },
	{ 0x26, "&amp;" },
	{ 0x22, "&quot;"}
};

static int ncodes = sizeof(he) / sizeof(struct iso8859_15);


/**************************************************************
						GENERAL GROUP
***************************************************************/	
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
// This one needs the struct and ncodes above.
// ncodes is the number of elements in the struct. 
char *htmlentities(const char *str)
{
	char *buf;
	int siz, len, i = 0, j;

	siz = strlen(str) + 1;
	
	buf = (char *)malloc(siz);	
	if (!buf)
		libcgi_error(E_MEMORY, "Failed to alloc memory at htmlentities, cgi.c");
	
	for (; *str; str++, i++)  {
		for (j = 0; j < ncodes; j++) {
			if (*str == he[j].code) {
				len = strlen(he[j].html) - 1;
				
				buf = realloc(buf, siz += len);	
				if (!buf)
					libcgi_error(E_MEMORY, "Failed to alloc memory at htmlentities, cgi.c");
				
				strcpy(buf + i, he[j].html);
				i += len;
				break;
			}
		}
			
		if (j == ncodes) 
			buf[i] = *str;
	}
	
	buf[i] = '\0';
	return buf;	
}

/**
* Reads entire file into an array.
* @param filename Filename to open
* @param total Integer variable passed as reference, which will store the total of items
* @return Returns the file in an array. Each element of the array corresponds to a line in the file.
* 
* \code
* char **lines;
* unsigned int total, i;
*  
* lines = file("filename.ext", &total);
*    
* printf("Total of lines: %u\n", total);
*    
* for (i = 0; i < total; i++)
*	printf("[%u] %s\n", i, lines[i]);
*     
* for (i = 0; i < total; i++) {
* 	if (lines[i])
*		 free(lines[i]);
* }
* \endcode	  
*/
char **file(const char *filename, unsigned int *total)
{
	FILE *fp;
	unsigned int lines, columms, char_count, i;
	char **str, *buf, ch;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		*total = 0;

		return NULL;
	}

	lines = 1;

	// initial line length
	columms = 100;

	// How many characteres in the line 
	char_count = 1;

	i = 0;

	// Allocate initial memory to str. Just one line...
	str = (char **)malloc(lines * sizeof(char *));
	if (!str)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	// Allocate initial memory to buf variable. It is the one 
	// that will contain the chars of the lines
	// By default, we're allocating 80 chars.. if more is needed, 
	// then we'll realloc()
	buf = (char *)malloc(columms);
	if (buf == NULL)
		libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

	while (!feof(fp)) {
		ch = fgetc(fp);
		
		// The next while() loop is  to get all contents of actual line 
		while ((ch != '\n') && (ch != EOF)) {
			// Increments the character counter
			char_count++;

			if (char_count >= columms) {
				columms += 20;
				buf = (char *)realloc(buf, columms);
				if (buf == NULL)
					libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);
			}
			buf[i++] = ch;

			ch = fgetc(fp);
		}

		buf[i] = '\0';

		str[lines - 1] = (char *)malloc(char_count);
		if (str[lines - 1] == NULL) {
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

			exit(EXIT_FAILURE);
		}

		strncpy(str[lines - 1], buf, char_count);
		memset(buf, 0, char_count);

		lines++;

		str = (char **)realloc(str, lines * sizeof(char *));
		if (str == NULL)
			libcgi_error(E_MEMORY, "%s, line %s", __FILE__, __LINE__);

		i = 0;
		char_count = 1;
	}

	free(buf);
	fclose(fp);

	*total = lines - 1;
	return str;	
}

/**
* @}
*/
/******************** GENERAL END ************************/
