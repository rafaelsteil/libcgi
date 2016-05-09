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
int delete_from_empty_list( void );
int delete_from_one_item_list( void );

int main( int argc, char *argv[] )
{
	struct cgi_test_action	actions[] = {
		{ "add",		add							},
		{ "deletezero",	delete_from_empty_list		},
		{ "deleteone",	delete_from_one_item_list	},
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

	/*	TODO	add tests for use with wrong arguments!	*/

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

int delete_from_empty_list( void )
{
	formvars	*start = NULL, *last = NULL;

	/*	add no item, try to delete an item	*/
	check( slist_delete( "not in there", &start, &last ) == 0, "not found" );

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int delete_from_one_item_list( void )
{
	const char	item_name[] = "item";
	formvars	*item, *start = NULL, *last = NULL;

	/*	add one item, delete one item	*/
	check( (item = calloc( 1, sizeof(formvars) )) != NULL, "calloc item" );
	check( (item->name = strdup( item_name )) != NULL, "strdup name item" );
	check( (item->value = strdup( "single" )) != NULL, "strdup value item" );
	slist_add( item, &start, &last );

	/*	try deleting not existing item and check list does not change	*/
	check( slist_delete( "not in there", &start, &last ) == 0, "not found" );
	check( start == item, "start == first item" );
	check( last == item, "last == first item" );
	check( item->next == NULL, "first item next == null" );

	/*	try deleting our item
	 *
	 *	NOTE	libcgi does not free the memory of deleted items	*/
	check( slist_delete( item_name, &start, &last ) == 1, "found" );
	check( start == NULL, "start = null" );
	check( last == NULL, "last = null" );

	free( item->name );
	free( item->value );
	free( item );
	item = NULL;

	/*	the list should already be empty	*/
	slist_free( &start );
	check( start == NULL, "slist_free reset start" );

	return EXIT_SUCCESS;

error:
	if ( item->name ) free( item->name );
	if ( item->value ) free( item->value );
	if ( item ) free( item );
	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
