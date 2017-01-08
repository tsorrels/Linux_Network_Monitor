

#define MAXROW 1000
#define MAXCOL 500


struct Header{
    int numrows;

};


struct State{

    struct Header header;
    
    WINDOW * scrn;

    int currow;
    int numlines;

    char ** lineoutput;
    
};


