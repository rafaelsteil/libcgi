/* test cases for module "cgi.c" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#define STDIN_FILENO _fileno(stdin)
#else
#include <unistd.h>
#endif

#include "cgi_test.h"

#include "cgi.h"

extern formvars *
process_data(const char *query, formvars **start, formvars **last,
             const char sep_value, const char sep_name);

static int pipe_[2];

static formvars *_post( const char *post_data );

static int test_cgi_escape_special_chars( void );
static int test_process_data( void );
static int test_cgi_param_multiple( void );
static int test_cgi_process_form( void );
static int _test_ltrim( void );
static int _test_rtrim( void );

int main( int argc, char *argv[] )
{
	struct cgi_test_action	actions[] = {
		{ "escape_special_chars",	test_cgi_escape_special_chars	},
		{ "process_data",			test_process_data				},
		{ "param_multiple",			test_cgi_param_multiple			},
		{ "process_form",			test_cgi_process_form			},
		{ "ltrim",					_test_ltrim						},
		{ "rtrim",					_test_rtrim						},
	};

	/*	require at least one argument to select test	*/
	if ( argc < 2 ) return EXIT_FAILURE;

#ifdef _MSC_VER
    check( !_pipe(pipe_, 1024, 0), "_pipe" );
#else
	check( !pipe(pipe_), "pipe" );
#endif
	check( STDIN_FILENO == dup2(pipe_[0], STDIN_FILENO), "dup2" );

	return run_action( argv[1], actions,
			sizeof(actions)/sizeof(struct cgi_test_action) );

error:
	return EXIT_FAILURE;
}

int test_cgi_escape_special_chars( void )
{
    puts(__FUNCTION__);

	const char *esc_valid = "%._-+0123456789"
	                        "abcdefghijklmnopqrstuvwxyz"
	                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char str[256];
	char *esc = NULL, *unesc = NULL;
	int c;

	for (c = 0; c < 256; ++c)
		str[c] = (char) c + 1;
	str[c] = 0;

	check( strlen(str) == 255, "strlen" );
	check( esc = cgi_escape_special_chars(str), "escape" );
	check( unesc = cgi_unescape_special_chars(esc), "unescape" );
	check( strcmp(esc, unesc), "strcmp esc unesc" );
	check( !strcmp(str, unesc), "strcmp str unesc" );
	check( strspn(esc, esc_valid) == strlen(esc), "strspn" );

	free( esc );
	free( unesc );
	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int test_process_data( void )
{
    puts(__FUNCTION__);

	formvars *fvp, *iter;
	const char *data[] = {"1=one&second=%20.two-%30&33=three+three",
	                      "&1=one&second=%20.two-%30&33=three+three",
	                      "=noname&1=one&&second=%20.two-%30&33=three+three",
	                      "==&&=&&1=one&second=%20.two-%30&==&33=three+three&=&",
	                      NULL};
	int i;

	for (i = 0; data[i]; ++i)
	{
		check( formvars_start == NULL, "start == NULL" );
		check( formvars_last == NULL, "last == NULL" );

		fvp = process_data(data[i], &formvars_start, &formvars_last, '=', '&');

		check( formvars_start && formvars_last && fvp,
				"all pointers not NULL" );
		check( fvp == formvars_start,
				"fvp == start" );

		check( !strcmp(fvp->name, "1"),
				"first element name" );
		check( !strcmp(fvp->value, "one"),
				"first element value" );
		check( (iter = fvp->next),
				"next pointer not NULL" );

		check( !strcmp(iter->name, "second"),
				"second element name" );
		check( !strcmp(iter->value, " .two-0"),
				"second element value" );
		check( (iter = iter->next),
				"next pointer not NULL" );

		check( !strcmp(iter->name, "33"),
				"third element name" );
		check( !strcmp(iter->value, "three three"),
				"third element value" );

		check( iter == formvars_last,
				"next pointer to last" );
		check( !(iter = iter->next),
				"last pointer NULL" );

		cgi_end();
	}

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int test_cgi_param_multiple( void )
{
    puts(__FUNCTION__);

	const char *data = "zero=0&one=one&one=two&two=three&one=four&three=3";

	process_data(data, &formvars_start, &formvars_last, '=', '&');

	check( !strcmp(cgi_param_multiple("one"), "one"),	"one" );
	check( !strcmp(cgi_param_multiple("one"), "two"),	"two" );
	check( !strcmp(cgi_param_multiple("one"), "four"),	"four" );
	check( !cgi_param_multiple("one"),					"one again" );

	cgi_end();

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

formvars *_post( const char *post_data )
{
	char length_str[20];
	size_t length = strlen(post_data);

	snprintf(length_str, sizeof(length_str),
			"%zd", length);

	check( !putenv("REQUEST_METHOD=POST"), "putenv REQUEST_METHOD" );
#ifndef _WIN32
	check( !setenv("CONTENT_LENGTH", length_str, 1), "CONTENT_LENGTH" );
#endif

	write(pipe_[1], post_data, length);

	check( formvars_start == NULL, "formvars_start" );
	check( formvars_last == NULL, "formvars_last" );

	return cgi_process_form();

error:
	return NULL;
}

int test_cgi_process_form( void )
{
    puts(__FUNCTION__);

	/* test no data */
	check( formvars_start == NULL,
			"start == NULL" );
	check( formvars_last == NULL,
			"last == NULL" );

	check( !putenv("QUERY_STRING"),
			"putenv" );
	check( !cgi_process_form(),
			"process form returns NULL" );

	check( !formvars_start && !formvars_last,
			"all pointers NULL" );

	cgi_end();

	/* test query string */
	check( formvars_start == NULL,
			"start == NULL" );
	check( formvars_last == NULL,
			"last == NULL" );

	check( !putenv("QUERY_STRING=zero=0&one=test"),
			"putenv" );
	check( cgi_process_form(),
			"process form returns valid pointer" );

	check( formvars_start && formvars_last,
			"all pointers not NULL" );
	check( !strcmp(formvars_start->name, "zero"),
			"first element name" );
	check( !strcmp(formvars_start->value, "0"),
			"first element value" );
	check( !strcmp(cgi_param("one"), "test"),
			"cgi param" );

	cgi_end();

	/* test post data */
	check( !putenv("QUERY_STRING"),
			"putenv" );
	check( _post("zero=0&one=test"),
			"POST" );

	check( formvars_start && formvars_last,
			"all pointers not NULL" );
	check( !strcmp(formvars_start->name, "zero"),
			"first element name" );
	check( !strcmp(formvars_start->value, "0"),
			"first element value" );
	check( !strcmp(cgi_param("one"), "test"),
			"cgi param" );

	cgi_end();

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int _test_ltrim( void )
{
    puts(__FUNCTION__);

    char before[][15] = {   "   test1",
                            "",
                            " ",
                            "        test",
                            "\nexample",
                            "\r\nline"      };
    char after[][15] =  {   "test1",
                            "",
                            "",
                            "test",
                            "example",
                            "line"          };
    size_t  n = sizeof(before) / 15;

    char buf[30];
    while (n)
    {
        --n;
        strcpy(buf, before[n]);
        cgi_ltrim(buf);
        check( !strcmp(buf, after[n]), "strcmp" );
    }

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

int _test_rtrim( void )
{
    puts(__FUNCTION__);

    char before[][10] = {   "test1",
                            "",
                            " ",
                            "test    ",
                            "example\n",
                            "line\r\n"      };
    char after[][10] =  {   "test1",
                            "",
                            "",
                            "test",
                            "example",
                            "line"          };
    size_t  n = sizeof(before) / 10;

    char buf[30];
    while (n)
    {
        --n;
        strcpy(buf, before[n]);
        cgi_rtrim(buf);
        check( !strcmp(buf, after[n]),	"first strcmp" );

        strcpy(buf, before[n]);
        cgi_rtrim(buf);
        check( !strcmp(buf, after[n]),	"second strcmp" );
    }

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}
