/*******************************************************************//**
 *	@file		cgi_test.c
 *
 *	Actual test routines checking stuff we want to check in unit tests.
 *
 *	@author		Alexander Dahl <post@lespocky.de>
 *
 *	SPDX-License-Identifier: LGPL-2.1+
 *	License-Filename: LICENSES/LGPL-2.1.txt
 *
 *	@copyright	2016 Alexander Dahl <post@lespocky.de>
 **********************************************************************/

#include "cgi_test.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*	***	declarations	***	*/

static int cmp_act( const void *e1, const void *e2 );

/*	***	definitions	***	*/

bool check_string_buffer_terminated( const char *buf, size_t len )
{
	size_t i;

	for ( i = 0; i < len; i++ )
	{
		if ( buf[i] == '\0' )
		{
			return true;
		}
	}

	return false;
}

int cmp_act( const void *e1, const void *e2 )
{
	struct cgi_test_action *p_act_1 = (struct cgi_test_action *) e1;
	struct cgi_test_action *p_act_2 = (struct cgi_test_action *) e2;

	return strcmp( p_act_1->action, p_act_2->action );
}

int run_action(
		const char				*action,
		struct cgi_test_action	actions[],
		size_t					len )
{
	struct cgi_test_action	*p_action;
	struct cgi_test_action	tmp_action = { NULL, NULL };

	/*	prepare lookup table	*/
	qsort( actions, len, sizeof(struct cgi_test_action), cmp_act );

	/*	search action in lookup table	*/
	tmp_action.action = action;
	p_action = (struct cgi_test_action *) bsearch( &tmp_action, actions,
			len, sizeof(struct cgi_test_action), cmp_act );

	/*	execute test	*/
	if ( p_action )
	{
		return p_action->func();
	}

	return EXIT_FAILURE;
}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
