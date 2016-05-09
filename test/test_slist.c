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
#include <string.h>

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
	int			i;
	formvars	*item[2] = { NULL, NULL }, *start = NULL, *last = NULL;

	check( (item[0] = calloc( 1, sizeof(formvars) )) != NULL, "calloc item 0" );
	check( (item[0]->name = strdup( "item 0" )) != NULL, "strdup name item 0" );
	check( (item[0]->value = strdup( "zero" )) != NULL, "strdup value item 0" );

	slist_add( item[0], &start, &last );
	check( start == item[0], "start == first item" );
	check( last == item[0], "last == first item" );
	check( item[0]->next == NULL, "first item next == null" );

	check( (item[1] = calloc( 1, sizeof(formvars) )) != NULL, "calloc item 1" );
	check( (item[1]->name = strdup( "item 1" )) != NULL, "strdup name item 1" );
	check( (item[1]->value = strdup( "one" )) != NULL, "strdup value item 1" );
	slist_add( item[1], &start, &last );
	check( start == item[0], "start == first item" );
	check( last == item[1], "last == first item" );
	check( item[0]->next == item[1], "first item next == item 1" );
	check( item[1]->next == NULL, "second item next == null" );

	slist_free( &start );
	check( start == NULL, "slist_free reset start" );

	return EXIT_SUCCESS;

error:
	for ( i = 0; i < 2; i++ ) {
		if ( item[i]->name ) free( item[i]->name );
		if ( item[i]->value ) free( item[i]->value );
		if ( item[i] ) free( item[i] );
	}
	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
