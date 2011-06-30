/*
	LibCGI base64 manipulation functions is extremly based on the work of Bob Tower,
	from its projec http://base64.sourceforge.net. The functions were a bit modicated. 
	Above is the MIT license from b64.c original code:

LICENCE:        Copyright (c) 2001 Bob Trower, Trantor Standard Systems Inc.

                Permission is hereby granted, free of charge, to any person
                obtaining a copy of this software and associated
                documentation files (the "Software"), to deal in the
                Software without restriction, including without limitation
                the rights to use, copy, modify, merge, publish, distribute,
                sublicense, and/or sell copies of the Software, and to
                permit persons to whom the Software is furnished to do so,
                subject to the following conditions:

                The above copyright notice and this permission notice shall
                be included in all copies or substantial portions of the
                Software.

                THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
                KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
                WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
                PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
                OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
                OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
                OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
                SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

void encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
	out[0] = cb64[ in[0] >> 2 ];
	out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
	out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
	out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

void decodeblock( unsigned char in[4], unsigned char out[3] )
{   
	out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
	out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
	out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/** 
* @ingroup libcgi_string
*/

/**
* Encodes a given tring to its base64 form.
* 
* @param *str String to convert
* @return Base64 encoded String
* @see str_base64_decode
**/
char *str_base64_encode(char *str)
{
	unsigned char in[3], out[4];
	unsigned int i, len, blocksout = 0, linesize = strlen(str);
	char *tmp = str;
	char *result = (char *)malloc((linesize + 3 - linesize % 3) * 4 / 3 + 1);
	
	if (!result)
		libcgi_error(E_MEMORY, "Failed to alloc memory at base64.c");
		
	while (*tmp) {
		len = 0;

		for( i = 0; i < 3; i++ ) {
			in[i] = (unsigned char)(*tmp);
			
			if (*tmp)
				len++;
			else
				in[i] = 0;
			
			tmp++;
		}
		
		if( len ) {
			encodeblock( in, out, len);
		
			for( i = 0; i < 4; i++ ) 
				result[blocksout++] = out[i];
		}		
	}
	
	result[blocksout] = '\0';
	return result;
}

/** 
* @ingroup libcgi_string
*/

/**
* Decode a base64 encoded string.
* 
* @param *str Encoded String to decode
* @return The decoded string
* @see str_base64_encode
**/
char *str_base64_decode(char *str)
{	
	unsigned char in[4], out[3], v;
	unsigned int i, len, pos = 0;
	char *tmp = str;
	
	char *result = (char *)malloc(strlen(str) + 1);
	if (!result)
		libcgi_error(E_MEMORY, "Failed to alloc memory at base64.c");
		
	while(*tmp) {
		for( len = 0, i = 0; i < 4 && *tmp; i++ ) {
			v = 0;
			
			while(*tmp && v == 0 ) {			
				v = (unsigned char)(*tmp);				
				v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);				
				
				if( v )
					v = (unsigned char) ((v == '$') ? 0 : v - 61);
					
				tmp++;
			}
			
			if(*tmp) {
				len++;
				
				if( v ) 
					in[i] = (unsigned char) (v - 1);
			}
			else 
				in[i] = 0;
		}
		
		if(len) {
			decodeblock( in, out );
			
			for( i = 0; i < len - 1; i++ )
				result[pos++] = out[i];
		}
	}

	result[pos] = '\0';
	return result;
}

