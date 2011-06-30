CC = gcc
FLAGS = -Wall
CGIBIN = .

all:
	$(CC) $(FLAGS) -lcgi session.c -o $(CGIBIN)/session.cgi
	$(CC) $(FLAGS) -lcgi login.c -o $(CGIBIN)/login.cgi
	$(CC) $(FLAGS) -lcgi show_vars.c -o $(CGIBIN)/show_vars.cgi
	$(CC) $(FLAGS) -lcgi restricted.c -o $(CGIBIN)/restricted.cgi
	$(CC) $(FLAGS) -lcgi destroy.c -o $(CGIBIN)/destroy.cgi

	@echo 
	@echo Example compiled.
	@echo Now copy *.cgi and *.html 
	@echo to your webserver cgi-bin directory
	@echo and call session.cgi application to test LibCGI session support
	@echo
