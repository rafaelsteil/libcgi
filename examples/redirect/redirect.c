
#include <stdio.h>
#include "cgi.h"

void example_description()
{
	puts("LibCGI Examples - cgi_redirect() function <br>"
	"This example show how to use cgi_redirect() to transfer <br>"
	"the actual page to an another<br><br>");
}

int main()
{
	cgi_init();
	cgi_process_form();

	if (cgi_param("url"))
		cgi_redirect(cgi_param("url"));
	
	cgi_init_headers();

	example_description();

	puts("<html>"
	"<head><title>Redirect example</title>"
	"</head>"
	"<body>"
	"<form action='redirect.cgi' method='GET'>"
	"URL to redirect: <input type=text name=url value='http://'><br>"
	"<input type=submit value='Click to redirect'>"
	"</form>"
	"</body>"
	"</html>");
	
	cgi_end();

	return 0;
}
