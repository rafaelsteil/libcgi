#include <stdio.h>
#include <cgi.h>
#include <session.h>

int main(void)
{
	cgi_init();
	cgi_session_start();
	cgi_process_form();
	
	// The user is trying to logon?
	if (cgi_param("action")) {
		// yeah!
		// in this case, we aren't looking for a correct
		// username or password, so anything is correct
		cgi_session_register_var("logged", "1");

		// Sends the user to main page
		cgi_redirect("session.cgi");
		cgi_end();
		
		return 0;
	}

	cgi_init_headers();

	puts(""
	"<html> "
	"<head><title>LibCGI session example - Login</title>"
	"</head>"
	"<body>"
	"");

	cgi_include("session_ex1_desc.html");

	puts(""
	"<table width='70%'>"
	"<tr>"
	"<td>"
	"");
	
	if (!cgi_session_var_exists("logged")) {
		puts(""
		"<form action='login.cgi' method='get'>"
		"<font face='arial, verdana' size='2'>To login, just type anything as 'username'</font>"
		"<br>"
		"Username: <input type='text' name='username'>"
		"<br>"
		"<input type='submit' name='action' value='Click to login'>"
		"</form>"
		"");
	}
	else {
		puts("<font face='arial, verdana' size='2'>You are already logged!!!</font>");
	}

	puts("</td></tr>");

	// navigation bar
	cgi_include("topbar.htm");

	puts("</table>"
	"</body>"
	"</html>");

	cgi_end();

	return 0;
}
