#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "error.h"
#include "cgi.h"

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
