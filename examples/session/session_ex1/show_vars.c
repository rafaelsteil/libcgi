#include <stdio.h>
#include <cgi.h>

int main(void)
{
	// formvars is a structure type used to manage session and cgi
	// linked lists. It have 'name' and 'value' properties. 
	formvars *sess_vars;
	char *data;
	
	cgi_init();
	cgi_session_start();	
	cgi_process_form();

	// The user is trying to unregister some variables?
	if (cgi_param("action")) {
		while ((data = cgi_param_multiple("unregister")))
			cgi_session_unregister_var(data);
	}

	cgi_init_headers();

	puts(""
	"<html>"
	"<head><title>LibCGI session example - variables</title>"
	"</head>"
	"<body>"
	"");

	cgi_include("session_ex1_desc.html");

	puts(""
	"<table width='70%' align='center'>"
	"<tr>"
	"<td>"
	"");

	if (!cgi_session_var_exists("logged")) {
		puts(""
		"<font face='arial, verdana' size='2'>You are not logged. To see this page contents, you"
		"must be logged. Please <a href='login.cgi'>click here</a> to go to login page."
		"</font>"
		"");
	}
	else {
		// show the registered session variables to user
		// sess_list_start is a global variable that contains 
		// all session variables information, such name and value
		sess_vars = sess_list_start;
		
		puts("<form action='show_vars.cgi' method='post'>");

		while (sess_vars) {
			printf(""
			"<font face='verdana, arial' size='2' color='#000000'>Variable: <b>%s</b></font>"
			"<font face='verdana, arial' size='2' color='#000000'>Value: <b>%s</b></font>&nbsp;&nbsp;"
			"<input type='checkbox' name='unregister' value='%s'>Unregister? <br>"
			"", sess_vars->name, sess_vars->value, sess_vars->name);

			sess_vars = sess_vars->next;
		}

		puts(""
		"<br><br>"
		"<input type='submit' name='action' value='Unregister selected'>"
		"</form>"
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
