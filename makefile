CC=gcc

CFLAGS = -g -c -Wall -pedantic
#CFLAGS = -ansi -c -Wall -pedantic

all: netmonitor

netmonitor: netmonitor.o
	    $(CC) -o netmonitor netmonitor.o -lncurses


clean:
	rm *.o
	rm netmonitor

%.o:    %.c
	$(CC) $(CFLAGS) $*.c


