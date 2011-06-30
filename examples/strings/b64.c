#include <stdio.h>
#include <cgi.h>
#include <string.h>
#include <stdlib.h>

char *example_description()
{
	char *desc = "
	LibCGI examples, str_base64_* functions demostration. <br>
	";
	
	return desc;
}

int main()
{
	char *to_encode, *to_decode;
	
	cgi_init();
	cgi_process_form();
	cgi_init_headers();
	
	if (cgi_param("action")) {
		to_encode = cgi_param("encode");
		to_decode = cgi_param("decode");
		
		// First check if we need to encode some string
		if (to_encode) {
			printf("<i>str_base64_encode()</i> example: original string: <b>%s</b><br>"
			"Encoded string: <b>%s</b><br><br>", to_encode, str_base64_encode(to_encode));
		}
		
		// Now if is to decode
		if (to_decode) {
			printf("<i>str_base64_decode()</i> example: original encoded string: <b>%s</b><br>"
			"Decoded string: <b>%s</b><br><br>", to_decode, str_base64_decode(to_decode));		
		}
		
		puts("<hr>");

		free(to_encode);
		free(to_decode);
	}
	
	printf(""
	"<html><head><title>LibCGI examples - str_base64_*()</title></head>"
	"<body>"
	"%s<br><br>"
	"<form action='b64.cgi' method='post'>"
	"String to encode: <input type='text' name='encode' size='30'><br>"
	"String to decode: <input type='text' name='decode' size='30'><br>"
	"<input type='submit' value='OK' name='action'>"
	"</form>"
	"</body>"
	"</html>"
	"", example_description());
	
	cgi_end();
	return 0;
}
