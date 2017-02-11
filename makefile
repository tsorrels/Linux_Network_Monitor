CC=gcc

CFLAGS = -g -c -Wall -pedantic
#CFLAGS = -ansi -c -Wall -pedantic

all: netmonitor

netmonitor: netmonitor.o systemdmod.o
	    $(CC) -o netmonitor netmonitor.o systemdmod.o -lncurses


clean:
	rm *.o
	rm netmonitor

%.o:    %.c
	$(CC) $(CFLAGS) $*.c


