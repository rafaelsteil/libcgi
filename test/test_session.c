/*******************************************************************//**
 *	@file		test_session.c
 *
 *	Test cgi session implementation.
 *
 *	@author		Alexander Dahl <post@lespocky.de>
 *
 *	SPDX-License-Identifier: LGPL-2.1+
 *	License-Filename: LICENSES/LGPL-2.1.txt
 *
 *	@copyright	2017 Alexander Dahl <post@lespocky.de>
 **********************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "cgi_test.h"

#include "libcgi/cgi.h"

#define CGI_TEST_SHRT_COOKIE_NAME	"cgi_sess"
#define CGI_TEST_COOKIE_NAME_49		"_______ten____twenty____thirty____fourty_____fift"
#define CGI_TEST_COOKIE_NAME_50		"_______ten____twenty____thirty____fourty_____fifty"
#define CGI_TEST_COOKIE_NAME_51		"_______ten____twenty____thirty____fourty_____fifty_"
#define CGI_TEST_LONG_COOKIE_NAME	"_______ten____twenty____thirty____fourty_____fifty_____sixty"

/*	local declarations	*/
static int cookie_name( void );

int main( int argc, char *argv[] )
{
	struct cgi_test_action	actions[] = {
		{ "cookie_name",	cookie_name	},
	};

	/*	require at least one argument to select test	*/
	if ( argc < 2 ) return EXIT_FAILURE;

	return run_action( argv[1], actions,
			sizeof(actions)/sizeof(struct cgi_test_action) );
}

int cookie_name( void )
{
// 	cgi_session_cookie_name( NULL );

	cgi_session_cookie_name( CGI_TEST_SHRT_COOKIE_NAME );
	check( strcmp( CGI_TEST_SHRT_COOKIE_NAME, SESSION_COOKIE_NAME ) == 0,
			"short cookie name not equal" );
	check( check_string_buffer_terminated( SESSION_COOKIE_NAME,
			sizeof(SESSION_COOKIE_NAME) ),
			"cookie name not terminated" );

	cgi_session_cookie_name( CGI_TEST_COOKIE_NAME_49 );
	check( strcmp( CGI_TEST_COOKIE_NAME_49, SESSION_COOKIE_NAME ) == 0,
			"49 cookie name not equal" );
	check( check_string_buffer_terminated( SESSION_COOKIE_NAME,
			sizeof(SESSION_COOKIE_NAME) ),
			"cookie name not terminated" );

	cgi_session_cookie_name( CGI_TEST_COOKIE_NAME_50 );
	check( check_string_buffer_terminated( SESSION_COOKIE_NAME,
			sizeof(SESSION_COOKIE_NAME) ),
			"cookie name not terminated" );

	cgi_session_cookie_name( CGI_TEST_COOKIE_NAME_51 );
	check( check_string_buffer_terminated( SESSION_COOKIE_NAME,
			sizeof(SESSION_COOKIE_NAME) ),
			"cookie name not terminated" );

	cgi_session_cookie_name( CGI_TEST_LONG_COOKIE_NAME );
	check( check_string_buffer_terminated( SESSION_COOKIE_NAME,
			sizeof(SESSION_COOKIE_NAME) ),
			"cookie name not terminated" );

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
