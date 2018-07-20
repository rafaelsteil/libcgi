/*******************************************************************//**
 *	@file	test_trim.c
 *
 *	Test different trim implementations.
 *
 *	@author	Alexander Dahl <post@lespocky.de>
 *
 *	Copyright 2017 Alexander Dahl
 **********************************************************************/

#include <stddef.h>
#include <string.h>

#include "cgi_test.h"

#include "libcgi/cgi.h"

/*	trim.c	*/
char* ltrim(char* s);
char* rtrim( char* s);
char*  trim( char* s);

/*	local declarations	*/
static int test_ltrim( void );
static int test_rtrim( void );
static int test_trim( void );

static const char *test_strings[] = {
	"   test1",
	"",
	" ",
	"        test",
	"\nexample",
	"\r\nline"
	"test1",
	"test    ",
	"example\n",
	"line\r\n",
	" \tfoo",
	"\tbar  \n",
	"\n\n",
	"\rbaz\r",
	NULL
};

int main( int argc, char *argv[] )
{
	struct cgi_test_action	actions[] = {
		{ "ltrim",	test_ltrim	},
		{ "rtrim",	test_rtrim	},
		{ "trim",	test_trim	},
	};

	/*	require at least one argument to select test	*/
	if ( argc < 2 ) return EXIT_FAILURE;

	return run_action( argv[1], actions,
			sizeof(actions)/sizeof(struct cgi_test_action) );
}

int test_ltrim( void )
{
	char		*str_1, *str_2;
	const char	**test_str;

	for ( test_str = test_strings; *test_str; test_str++ )
	{
		check( (str_1 = strdup( *test_str )), "strdup failed" );
		check( (str_2 = strdup( *test_str )), "strdup failed" );

		check( cgi_ltrim( str_1 ) == str_1, "cgi_ltrim" );
		check( ltrim( str_2) == str_2, "ltrim" );

		check( !strcmp( str_1, str_2 ), "strcmp" );

		free( str_1 );
		free( str_2 );
	}

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int test_rtrim( void )
{
	char		*str_1, *str_2;
	const char	**test_str;

	for ( test_str = test_strings; *test_str; test_str++ )
	{
		check( (str_1 = strdup( *test_str )), "strdup failed" );
		check( (str_2 = strdup( *test_str )), "strdup failed" );

		check( cgi_rtrim( str_1 ) == str_1, "cgi_rtrim" );
		check( rtrim( str_2) == str_2, "rtrim" );

		check( !strcmp( str_1, str_2 ), "strcmp" );

		free( str_1 );
		free( str_2 );
	}

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int test_trim( void )
{
	char		*str_1, *str_2;
	const char	**test_str;

	for ( test_str = test_strings; *test_str; test_str++ )
	{
		check( (str_1 = strdup( *test_str )), "strdup failed" );
		check( (str_2 = strdup( *test_str )), "strdup failed" );

		check( cgi_trim( str_1 ) == str_1, "cgi_trim" );
		check( trim( str_2) == str_2, "trim" );

		check( !strcmp( str_1, str_2 ), "strcmp" );

		free( str_1 );
		free( str_2 );
	}

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
