#include <stdio.h>
#include <cgi.h>
#include <session.h>

int main(void)
{
	cgi_init();
	cgi_session_start();
	cgi_init_headers();

	puts("<html><body>");
	cgi_include("session_ex1_desc.html");
	
	puts(""
	"<table width='70%%' align='center'>"
	"<tr>"
	"<td align='center'>"
	"<font face='arial, verdana' size='4' color='orange'>Main page"
	"");

	// The user is logged?
	if (cgi_session_var_exists("logged")) 
		puts(" - logged");
	else
		puts(" - not logged yet");

	puts(""
	"</font>"
	"</td>"
	"</tr>"
	"	"
	"<tr>"
	"<td><font face='arial, verdana' size='2' color='#0000ff'>"
	"This is the main page of LibCGI session suport test. Please choose one of the options"
	"in the navigation bar.</font>"
	"</td>"
	"</tr>"
	"");

	// include the navigation bar
	cgi_include("topbar.htm");

	puts(""
	"</table>"
	"</body>"
	"</html>"
	"");

	cgi_end();

	return 0;
}
