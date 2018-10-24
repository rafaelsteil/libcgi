/*
 * LibCGI - A library to make CGI programs using C
 *
 * Copyright 2001 Rafael Steil <rafael@insanecorp.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 * License-Filename: LICENSES/LGPL-2.1.txt
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "libcgi/cgi.h"
#include "libcgi/error.h"

const char *libcgi_error_type[] = {
	"LibCGI Warning",
	"LibCGI Fatal",
	"LibCGI Caution",
	"LibCGI Information",
	"LibCGI out of memory"
};

void libcgi_error(int error_code, const char *msg, ...)
{
	va_list arguments;

	if (!cgi_display_errors)
		return;

	cgi_init_headers();
	va_start(arguments, msg);

	printf("<b>%s</b>: ", libcgi_error_type[error_code]);
	vprintf(msg, arguments);
	puts("<br>");

	va_end(arguments);

	if ((error_code == E_FATAL) || (error_code == E_MEMORY)) {
		cgi_end();

		exit(EXIT_FAILURE);
	}
}
