/*******************************************************************//**
 *	@file	cgi_test.h
 *
 *	Macros for test routines, inspired by »Learn C The Hard Way« …
 *
 *	@see	http://c.learncodethehardway.org/
 *
 *	@author	Alexander Dahl <post@lespocky.de>
 *
 *	Copyright 2015,2016 Alexander Dahl
 **********************************************************************/

#ifndef CGI_TEST_H
#define CGI_TEST_H

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define clean_errno()		(errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...)		fprintf( stderr, \
									"[ERROR] (%s:%d: errno: %s) " M "\n", \
									__FILE__, __LINE__, clean_errno(), \
									##__VA_ARGS__)

#define check(A, M, ...)	if(!(A)) { \
									log_err(M, ##__VA_ARGS__); \
									errno=0; goto error; }

typedef int (*t_cgi_test_action_func)( void );

struct cgi_test_action {
	const char				*action;
	t_cgi_test_action_func	func;
};

#ifdef __cplusplus
extern "C" {
#endif

bool check_string_buffer_terminated( const char *buf, size_t len );

int run_action(
		const char				*action,
		struct cgi_test_action	actions[],
		size_t					len
);

#ifdef __cplusplus
}
#endif

#endif /* CGI_TEST_H */

/* vim: set noet sts=0 ts=4 sw=4 sr: */
