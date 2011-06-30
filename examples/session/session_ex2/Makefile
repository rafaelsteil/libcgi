CC = gcc
FLAGS = -Wall
CGIBIN = .

all:
	$(CC) $(FLAGS) -lcgi session_ex2.c -o $(CGIBIN)/session_ex2.cgi

	@echo 
	@echo Example compiled.
	@echo Now copy *.cgi and *.html 
	@echo to your webserver cgi-bin directory, create a directory called
	@echo \"session_files\" there, give permission to the webserver\'s user
	@echo and call session_ex2.cgi application to test LibCGI session support
	@echo
