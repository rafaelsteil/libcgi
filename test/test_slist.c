/*******************************************************************//**
 *	@file	test_slist.c
 *
 *	Test cgi slist implementation.
 *
 *	@author	Alexander Dahl <post@lespocky.de>
 *
 *	Copyright 2016 Alexander Dahl
 **********************************************************************/

#include <stddef.h>

#include "cgi_test.h"

#include "cgi.h"

int add( void );

int main( int argc, char *argv[] )
{
	struct cgi_test_action	actions[] = {
		{ "add",	add	},
	};

	/*	require at least one argument to select test	*/
	if ( argc < 2 ) return EXIT_FAILURE;

	return run_action( argv[1], actions,
			sizeof(actions)/sizeof(struct cgi_test_action) );
}

int add( void )
{
	formvars *item = NULL, *start = NULL, *last = NULL;

	check( (item = calloc( 1, sizeof(formvars) )) != NULL, "calloc" );

	/*	TODO	add checks and actually test slist_add() !!	*/

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
