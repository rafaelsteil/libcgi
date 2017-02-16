/*
    LibCGI - A library to make CGI programs using C
    Copyright (C) 2001 Rafael Steil
    Copyright 2016 Alexander Dahl <post@lespocky.de>

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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "cgi.h"

// Add a new item to the list
void slist_add(formvars *item, formvars **start, formvars **last)
{
	// if *start is empty, then our list is also empty. So, the only
	// task to do is fill the variables *start and *last with the data pointed by item
	// *start contains the firts item in the list, and we need *last to know the
	// end of the list
	if (!*start) {
		// item->next needs to be NULL, otherwise we never will
		// find the end of list
		item->next = NULL;

		*start = item;
		*last = item;

		return;
	}

	// Ok, in this case, the next item will contain the
	// new data. Note that, before it, (*last)->next contains
	// NULL ( the end of the list )
	(*last)->next = item;

	// The 'next' argument of item variable need to contain
	// NULL, or in other words, it will be the end ;0
	item->next = NULL;

	// Finaly, the actual end of list will contain the new item
	*last = item;
}

// Delete from list the item pointed by name
int slist_delete(char *name, formvars **start, formvars **last)
{
	formvars *curr, *prev;

	if ( !name ) return 0;

	for ( prev = NULL, curr = *start;
			curr;
			prev = curr, curr = curr->next )
	{
		if ( !strcasecmp( curr->name, name ) )
		{
			if ( prev == NULL )
			{
				/*	first element matches	*/
				*start = curr->next;
			}
			else
			{
				/*	relink list aka skip the to be deleted element	*/
				prev->next = curr->next;
			}

			if ( curr->next == NULL )
			{
				*last = prev;
			}

			/*	deallocate current element	*/
			free( curr->name );
			free( curr->value );
			free( curr );

			return 1;
		}
	}

	return 0;
}

// Returns the value of the item pointed by name
char *slist_item(const char *name, formvars *start)
{
	formvars *curr = start;

	if ( !name ) return NULL;

	while ( curr ) {
		if ( !strcasecmp( curr->name, name ) ) {
			if ( curr->value == NULL || curr->value[0] == '\0' ) {
				return NULL;
			} else {
				return curr->value;
			}
		}

		curr = curr->next;
	}

	return NULL;
}

// Free linked list allocated memory
void slist_free(formvars **start)
{
	while (*start) {
		void *p = *start;
		free((*start)->name);
		free((*start)->value);

		*start = (*start)->next;

		free(p);
	}

	*start = NULL;
}

