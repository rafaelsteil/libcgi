#include <stdio.h>
#include <cgi.h>
#include <session.h>

int main(void)
{
	char *name, *value;
	
	cgi_init();
	cgi_session_start();
	cgi_process_form();

	// The user is trying to register a variable?
	if (cgi_param("action")) {
		name = cgi_param("var_name");
		value = cgi_param("var_value");

		if (name && value) {
			cgi_session_register_var(name, value);
			cgi_redirect("show_vars.cgi");
			cgi_end();

			return 0;
		}
	}

	cgi_init_headers();

	puts(""
	"<html>"
	"<head><title>LibCGI session examples - Restricted area</title>"
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
		puts(""
		"<font face='arial, verdana' size='2'>To see this page contents, you must be logged."
		"<a href='login.cgi'>Click here</a> to go to login page."
		"</font>"
		"");
	}
	else {
		puts(""
		"<font face='arial, verdana' size='2' color='#ff0000'>In this page you can register"
		"any session variable you want. Here is a good place to test session suport, and "
		"<a href='show_vars.cgi'>here</a> you can see all already registered variables!!!|"
		"</font>"
		"<br>"
		"<font face='arial, verdana' size='2'><b>To registed a variable, fill the following fields: </b></font>"
		"<br>"
		"<form action='restricted.cgi' method='pos'>"
		"<font face='verdana, arial' size='1'>Variable name: <input type='text' name='var_name'><br>"
		"Variable value: <input type='text' name='var_value'><br>"
		"</font>"
		"<input type='submit' name='action' value='Click to register'>"
		"</form>"
		"");
	}

	cgi_include("topbar.htm");

	puts(""
	"</td>"
	"</tr>"
	"</table>"
	"</body>"
	"</html>"
	"");

	cgi_end();

	return 0;
}
