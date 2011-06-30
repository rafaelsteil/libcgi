#include <stdio.h>
#include "cgi.h"

int main(void)
{
	cgi_init();
	cgi_process_form();
	cgi_init_headers();

	puts(""
	"<!DOCTYPE html PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'>"
	"<html>"
	"<head>"
	"	"
	 " <meta http-equiv='content-type' content='text/html; charset=ISO-8859-1'>"
	"	"
	 "   <meta name='author' content='Rafael Steil'>"
	  "    <title>LIBCGI Examples</title>"
	   "   </head>"
	   "  <body text='#000000' bgcolor='#ffffff' link='#0000ee' vlink='#551a8b' alink='#0000ee'>"		
	"");

	// NAME
	if (cgi_param("name"))
		printf("Name: %s<br>", cgi_param("name"));
	else
		puts("Name: Empty<br>");

	// AGE
	if (cgi_param("age"))
		printf("Age: %s<br>", cgi_param("age"));
	else
		puts("Age: Emtpy<br>");

	// GENDER
	printf("Gender: %s<br>", cgi_param("gender"));

	// RADIO BUTTON 1
	printf("Radio Button 1: %s<br>", cgi_param("radiobutton1"));

	// RADIO BUTTON 2
	printf("Radio button 2: %s<br>", cgi_param("radiobutton2"));

	puts(""
	"</body>"
	"</html>"
	"");	

	cgi_end();
	
	return 0;
}
