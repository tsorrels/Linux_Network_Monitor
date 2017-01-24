

#define MAXROW 1000
#define MAXCOL 500
#define NUMHEADROWS 5

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
  
    pid_t pidkill;
  
};
