#include <stdio.h>
#include <time.h>
#include <cgi.h>

// File to write the messages
#define GBOOK "/tmp/libcgi_gbook.html"

void show_comments()
{
	// stupid code :)
	cgi_include(GBOOK);
}

char *now()
{
	struct tm *now;
	time_t now_time_t;
	char *buffer;

	now_time_t = time(NULL);
	now = localtime(&now_time_t);

	buffer = asctime(now);

	return buffer;
}

void example_description()
{
	puts(" LibCGI Examples - Extreme simple GuestBook<br> "
	"This example shows how to use cgi_include() function to show the<br>"
	"user some HTML data contained in another file. <br><br>");
}

int main()
{	
	cgi_init();
	cgi_process_form();	
	cgi_init_headers();

	example_description();

	if (cgi_param("action")) {
		// write the msg to the file
		FILE *fp;

		fp = fopen(GBOOK, "a");
		if (!fp) 
			cgi_fatal("Failed to open guestbook file for appending");

		fseek(fp, 0, SEEK_SET);

		fprintf(fp, "<b>Date:</b> %s<br>", now());
		fprintf(fp, "<b>Name:</b> %s<br>", cgi_param("name"));
		fprintf(fp, "<b>E-Mail: </b>%s<br>", cgi_param("email"));
		fprintf(fp, "<b>Message: </b>%s<br><br>", cgi_param("msg"));

		fclose(fp);
	}

	puts("<html><head><title>LibCGI examples - GuestBook</title></head><body>");
	cgi_include("table_add_comments.html");
	puts("<table width='70%' align='center' border='0'><tr><td>");

	show_comments();

	puts("</td></tr></table></body></html>");
	
	cgi_end();
	return 0;
}
