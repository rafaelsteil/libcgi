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

// This code is a bit complicated, and I needed some 
// long hours to terminate it. In a future release, 
// I will explain the algorithm better
int slist_delete(char *name, formvars **start, formvars **last)
{
	// *begin will be used to store the start of the list,
	// *prior will store the prior item relacted to actual in the loop
	formvars *begin, *prior;

	// Before of all, is more simple to check if the item 
	// to delete is in the next item. If true, we don't need
	// to enter in the loop
	if (!strcasecmp((*start)->name, name)) {
		// The start of the list will contains the next item
		*start = (*start)->next;

		// if start is null, then we haven't more itens
		// in the list.. 
		if (!*start) 
			*last = NULL;

		return 1 ;
	}

	// Stores the start of the list
	begin = *start;

	// Stays in the loop while the item to be deleted 
	// is not found
	while (*start) {
		// Stores the prior item of the list
		// Note that, in the loop, we're really changing
		// the value of start variable, then it works correctly
		prior = *start;

		// Critical section:
		// the next item is the one to be deleted???
		if (!strcasecmp((*start)->next->name, name)) {
			// Before, check if the item that will be deleted
			// is the last... if true, then the next item need to 
			// contain null ( the end of list ), and the actual
			// value is changed with the value of the prior variable
			if ((*start)->next == *last) {
				(*start)->next = NULL;
				*last = prior;
			}			
			else
				// otherwise... well
				// the item that will be deleted is pointed
				// by (*start)->next. So, the item after them
				// is the new item in this position
				// Note:
				// ...
				// [1] item one
				// [2] item two <-- This one will be deleted (example)
				// [3] item three
				// [4] item four
				// [5] item five
				//
				// So, now the list is
				// [1] item one
				// [3] item three
				// [4] item four
				// [5] item five
				//
				// but the above is only a simple example, because the
				// position of the itens is changed too
				// [1] item one
				// [2] item three
				// [3] item four
				// [4] item five
				//
				// well.. the basic idea is it...
				(*start)->next = (*start)->next->next;

			// Restores the start of the list
			*start = begin;

			// finish
			return 1;
		}

		// Walk to the next item
		*start = (*start)->next;
	}

	*start = begin;

	return 0;
}

// Returns the value of the item pointed by name
char *slist_item(const char *name, formvars *start)
{
	formvars *begin;
	begin = start;

	while (begin) {
		if (!strcasecmp(begin->name, name)) 
			return (!begin->value[0] ? NULL : begin->value);

		begin = begin->next;
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

