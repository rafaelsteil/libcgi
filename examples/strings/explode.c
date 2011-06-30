#include <stdio.h>
#include <stdlib.h>
#include <cgi.h>

char *example_description()
{
	char *desc = ""
	"LibCGI examples, explode() function demonstration. <br>"
	"Type a string using wildcards and type the wildcard you would<br>"
	"like to use, and then click the submit button to split the<br>"
	"string in pieces."
	"";
	
	return desc;
}

int main()
{
	char *str, *delim, **e;
	
	unsigned int total, i;
	
	cgi_init();
	cgi_process_form();
	cgi_init_headers();
	
	// The form was submited??
	if (cgi_param("action")) {
		str = cgi_param("str");
		delim = cgi_param("delim");
		
		e = explode(str, delim, &total);
		
		if (total < 1)
			puts("explode() returned 0 items.");
		else {
			for (i = 0; i < total; i++)
				printf("Item offset [%d]: %s<br>", i, e[i]);
		}
		
		free(str);
		free(delim);
	}
	else {
		printf(""
		"<html><head><title>LibCGI examples - explode()</title></head>"
		"<body>"
		"%s<br><br>"
		"<form action='explode.cgi' method='post'>"
		"String: <input type='text' name='str' value='this,is,a,test'><br>"
		"Delimitator: <input type='text' name='delim' value=',' maxlength='1' size='2'><br>"
		"<input type='submit' value='explode() the string!!!' name='action'>"
		"</form>"
		"</body>"
		"</html>"
		"", example_description());
	}
	
	cgi_end();
	return 0;
}
	
