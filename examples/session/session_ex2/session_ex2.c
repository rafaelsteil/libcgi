#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cgi.h>
#include <session.h>

int main()
{
	char *action;
	
	cgi_init();
	cgi_process_form();

	// session id will be stored in a cookie called "MY_COOKIE"
	cgi_session_cookie_name("MY_COOKIE");
	// tell LibCGI to store session files into "session_files" directory
	cgi_session_save_path("session_files/");

	// Starts the session
	cgi_session_start();

	// What is to do??
	action = cgi_param("action");
	if (action) {
		// login
		if (!strcasecmp("login", action) && cgi_param("str"))
			cgi_session_register_var("str", cgi_param("str"));
		// logoff
		else if (!strcasecmp("logoff", action))
			cgi_session_destroy();
	}

	cgi_init_headers();

	// write initial html code
	puts("<html><body>");

	// includes the description for this example
	cgi_include("session_ex2_desc.html");
  	
	// The user is logged?
	if (cgi_session_var_exists("str")) {
		// yep, let's show him some things
		
		printf(""
		"<br><b>Congratulations, you've logged</b><br>"
		"If you want to finish the session, <a href='session_ex2.cgi?action=logoff'>click here</a>!"
		"<br><br>"
		"You pass phrase was <b>%s</b><br>"
		"", cgi_session_var("str"));
	}
	// Nope, he isn't
	else {
		puts(""
			"<form action='session_ex2.cgi' method='get'>"
			"	Pass phrase: <input type='text' name='str'>"
			"	<input type='submit' name='submit' value='Click to login'>"
			"	<input type='hidden' name='action' value='login'>"
			"</form>"
		"");
	}		

	// Print session configuration directives
	printf("Session configuration options: <br>"
		"saving session files at: <b>%s</b> directory<br>"
		"name of the (personalized) cookie: <b>%s</b><br>", SESSION_SAVE_PATH, SESSION_COOKIE_NAME);

	puts("</body></thml>");

	if (action)
		free(action);

	cgi_end();
	return 0;
}

