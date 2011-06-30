#include <stdio.h>
#include <cgi.h>

char *example_description()
{
	char *desc = ""
	"LibCGI examples, htmltities() function demostration. <br>"
	"Type an html code into textarea and then click OK"
	"";
	
	return desc;
}

int main()
{
	char *q;
	
	cgi_init();
	cgi_process_form();
	cgi_init_headers();
	
	q = cgi_param("q");
	
	if (q)
		puts(htmlentities(q));
	else {
		printf(""
		"<html><head><title>LibCGI examples - htmlentities()</title></head>"
		"<body>"
		"%s<br><br>"
		"<form action='htmlentities.cgi' method='post'>"
		"HTML code: <textarea rows='8' name='q'><html><body>test, test</body></html></textarea><br>"
		"<input type='submit' value='OK' name='action'>"
		"</form>"
		"</body>"
		"</html>"
		"", example_description());	
	}
	
	cgi_end();
	return 0;
}
