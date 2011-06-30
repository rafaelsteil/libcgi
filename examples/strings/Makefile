CC = gcc
FLAGS = -Wall
CGIBIN = .

all:

	$(CC) $(FLAGS) -lcgi explode.c -o $(CGIBIN)/explode.cgi
	$(CC) $(FLAGS) -lcgi htmlentities.c -o $(CGIBIN)/htmlentities.cgi
	$(CC) $(FLAGS) -lcgi b64.c -o $(CGIBIN)/b64.cgi
	$(CC) $(FLAGS) -lcgi md5.c -o $(CGIBIN)/md5.cgi

	@echo 
	@echo Example compiled. Now copy *.cgi and *.htm files 
	@echo to your webserver cgi-bin directory.
	@echo 

