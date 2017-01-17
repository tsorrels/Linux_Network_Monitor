

#define MAXROW 1000
#define MAXCOL 500


struct Header{
    int numrows;

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
