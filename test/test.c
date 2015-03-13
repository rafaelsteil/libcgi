/* test cases for module "cgi.c" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cgi.h"

extern formvars *
process_data(char *query, formvars **start, formvars **last,
             const char delim, const char sep);

static void
test_cgi_escape_special_chars(void)
{
	const char *esc_valid = "%._-+0123456789"
	                        "abcdefghijklmnopqrstuvwxyz"
	                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char str[256];
	char *esc, *unesc;
	int c;

	for (c = 0; c < 256; ++c)
		str[c] = (char) c + 1;
	str[c] = 0;

	assert(strlen(str) == 255);

	assert(esc = cgi_escape_special_chars(str));
	assert(unesc = cgi_unescape_special_chars(esc));
	assert(strcmp(esc, unesc));
	assert(! strcmp(str, unesc));
	assert(strspn(esc, esc_valid) == strlen(esc));
}

static void
test_process_data(void)
{
	formvars *fvp, *iter;
	const char *data[] = {"1=one&second=%20.two-%30&33=three+three",
	                      "&1=one&second=%20.two-%30&33=three+three",
	                      "=noname&1=one&&second=%20.two-%30&33=three+three",
	                      "==&&=&&1=one&second=%20.two-%30&==&33=three+three&=&",
	                      NULL};
	int i;

	for (i = 0; data[i]; ++i)
	{
		assert(formvars_start == NULL);
		assert(formvars_last == NULL);

		fvp = process_data(data[i], &formvars_start, &formvars_last, '=', '&');

		assert(formvars_start && formvars_last && fvp);
		assert(fvp == formvars_start);

		assert(! strcmp(fvp->name, "1"));
		assert(! strcmp(fvp->value, "one"));
		assert(iter = fvp->next);

		assert(! strcmp(iter->name, "second"));
		assert(! strcmp(iter->value, " .two-0"));
		assert(iter = iter->next);

		assert(! strcmp(iter->name, "33"));
		assert(! strcmp(iter->value, "three three"));

		assert(iter == formvars_last);
		assert(! (iter = iter->next));

		cgi_end();
	}
}

void
test_cgi_param_multiple(void)
{
	const char *data = "zero=0&one=one&one=two&two=three&one=four&three=3";

	process_data(data, &formvars_start, &formvars_last, '=', '&');

	assert(! strcmp(cgi_param_multiple("one"), "one"));
	assert(! strcmp(cgi_param_multiple("one"), "two"));
	assert(! strcmp(cgi_param_multiple("one"), "four"));
	assert(! cgi_param_multiple("one"));

	cgi_end();
}

void
test_cgi_process_form(void)
{
	/* test no data */
	assert(formvars_start == NULL);
	assert(formvars_last == NULL);

	assert(! putenv("QUERY_STRING"));
	assert(! cgi_process_form());

	assert(! formvars_start && ! formvars_last);
	cgi_end();

	/* test query string */
	assert(formvars_start == NULL);
	assert(formvars_last == NULL);

	assert(! putenv("QUERY_STRING=zero=0&one=one"));
	assert(cgi_process_form());

	assert(formvars_start && formvars_last);
	assert(! strcmp(formvars_start->name, "zero"));
	assert(! strcmp(formvars_start->value, "0"));

	cgi_end();
}



/*****************************************************************************/

int
main(void)
{
	test_cgi_escape_special_chars();
	test_process_data();
	test_cgi_param_multiple();
	test_cgi_process_form();

	puts("Tests passed.");
	return 0;
}

