#include <stdio.h>
#include <cgi.h>
#include <session.h>
#include <string.h>

int main(void)
{
	cgi_init();
	cgi_session_start();
	cgi_process_form();

	// Is to destroy the session?
	if (cgi_param("confirm") && !strcmp(cgi_param("confirm"), "yes")) {
		cgi_session_destroy();
		cgi_end();		
		cgi_redirect("session.cgi");
		return 0;
	}

	cgi_init_headers();

	puts(""
	"<html>"
	"<head><title>LibCGI session examples - Destroy session</title>"
	"</head>"
	"<body>"
	"");

	cgi_include("session_ex1_desc.html");
	
	puts(""
	"<table width='70%%' align='center'>"
	"<tr>"
	"<td>"
	"");

	if (!cgi_session_var_exists("logged")) {
		puts("<font face='arial, verdana' size='2'>You are not logged yet</font>");
	}
	else {
		puts(""
		"<font face='arial, verdana' size='2'>If you are sure to unregister the session, "
		"<a href='destroy.cgi?confirm=yes'>click here</a></font>"
		"");
	}

	puts("</td></tr>");

	cgi_include("topbar.htm");

	puts(""
	"</table>"
	"</body>"
	"</html>"
	"");

	cgi_end();

	return 0;
}
		
