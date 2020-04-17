all: chat_server

CC=gcc
CFLAGS=-pedantic -Wall
#For debugging un-comment the next line
#DFLAGS=-DDEBUG_TRACE=1 -g
INCLUDE_PATH=../chat_app_with_qt4/.
MYSQL_INCLUDE_PATH=/usr/include/mysql
MYSQL_LIB_PATH=/usr/lib/mysql
MYSQL_LIB_FILE=mysqlclient 
#on some distributions you may need to change the above line to include
#the compatability library, as shown below.
#LIBS=gdbm

.c.o:
	$(CC) $(CFLAGS) -I$(MYSQL_INCLUDE_PATH) $(DFLAGS) -c $<

chat_server.o: chat_server.c rdwrt_lib.h ../chat_app_with_qt4/client_server_comm.h
rdwrt_lib.o: rdwrt_lib.c rdwrt_lib.h


chat_server: chat_server.o rdwrt_lib.o
	    $(CC) -o chat_server -I$(INCLUDE_PATH) -L$(MYSQL_LIB_PATH) $(DFLAGS) chat_server.o rdwrt_lib.o -l$(MYSQL_LIB_FILE)

clean:
	rm -f chat_server *.o *~