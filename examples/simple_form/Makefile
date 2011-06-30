CC = gcc
FLAGS = -Wall
CGIBIN = .

all:
	$(CC) $(FLAGS) -lcgi form1.c -o $(CGIBIN)/form1.cgi
	$(CC) $(FLAGS) -lcgi form2.c -o $(CGIBIN)/form2.cgi

	@echo
	@echo Example compiled.
	@echo Now copy *.cgi and *.inc files
	@echo to your webserver cgi-bin directory
	@echo and call form1.cgi application
	@echo
