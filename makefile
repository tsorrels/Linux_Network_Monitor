CC=gcc

CFLAGS = -g -c -Wall -pedantic
#CFLAGS = -ansi -c -Wall -pedantic

all: netmonitor

netmonitor: netmonitor.o systemdmod.o log.o
	    $(CC) -o netmonitor netmonitor.o systemdmod.o log.o -lncurses -lpthread


clean:
	rm *.o
	rm *.log
	rm netmonitor

%.o:    %.c
	$(CC) $(CFLAGS) $*.c


