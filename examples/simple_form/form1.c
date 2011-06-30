#include <stdio.h>
#include "cgi.h"

void example_description()
{
	puts(""
	"LibCGI Examples - Simple form<br>"
	"This example shows you various ways to manipulate HTML<br>"
	"forms and data<br><br>"
	"");
}

int main(void)
{
	cgi_init();
	cgi_init_headers();

	example_description();

	// Init the HTML page
	puts(""
	"<!DOCTYPE html PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'>"
	"<html>"
	"<head>"
	" "
	  "<meta http-equiv='content-type' content='text/html; charset=ISO-8859-1'>"
	"	"
	 "   <meta name='author' content='Rafael Steil'>"
	  "    <title>LIBCGI Examples</title>"
	   "   </head>"
	    "  <body text='#000000' bgcolor='#ffffff' link='#0000ee' vlink='#551a8b' alink='#0000ee'>"
	     " <table cellpadding='0' cellspacing='0' border='0' width='90%' align='Center' bgcolor='#33cc00'>"
	      "  <tbody>"
	"	  <tr>"
	"");
	
	cgi_include("top.inc");
	cgi_include("left.inc");
	cgi_include("main.inc");

	puts(""
	   "</tr>"
	   " </tbody>"
	   "</table>"
	   "</body>"
	   "</html>"
	"");
	
	cgi_end();

	return 0;
}
