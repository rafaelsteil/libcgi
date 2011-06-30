#include <stdio.h>
#include <cgi.h>

void example_description()
{
	puts("LibCGI examples - Fetching multiple-chekboxes with same name<br>"
	"This example shows how cgi_param_multiple() function works. <br>"
	"If you look at form.html file, you'll see that all chekboxes<br>"
	"have the same name. <br><br>");
}

int main(void)
{
	char *data;
		
	cgi_init();
	cgi_init_headers();
	cgi_process_form();

	example_description();

	if (cgi_param("action")) {
		while ((data = cgi_param_multiple("check")) != NULL)
			printf("%s<br>", data);
	}
	else 
		cgi_include("form.html");

	cgi_end();
	return 0;
}
