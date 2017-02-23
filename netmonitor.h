#include <sys/types.h>
#include <curses.h>

#define MAXROW 1000
#define MAXCOL 500
#define NUMHEADROWS 5
#define SIZETIMEBUF 32

#define SIZELOGBUF 128

struct Header{
    int numrows;

    char netdevoutput[NUMHEADROWS + 2][MAXCOL];

    char message[MAXCOL];

};


struct State{

    struct Header header;
    
    WINDOW * scrn;

    int curline;
    int currow;
    int numlines;

    char lineoutput[MAXROW][MAXCOL];
  
    char logbuffer[SIZELOGBUF];

    pid_t pidkill;
    int paused; 
 
};



#define ERRSYSDSTOP 1
#define ERRSYSDNAME 2

#define ERROS 1

#define MAXLOG 200

#define LOGERROR 0
#define LOGEVENT 1


int ismanaged(pid_t pid);
int stopservice(pid_t pid);
