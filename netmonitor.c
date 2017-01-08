/* Idea borrowed from Norman Matloff, UC Davis */




#include <curses.h>
#include "netmonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct State state;

void quit(int status);
void initialize();
void writeheader();
void writelines();


void initialize(){

    state.scrn = initscr();

    state.header.numrows = 5;
    state.lineoutput = malloc(MAXROW * MAXCOL * sizeof(char));
    if (state.lineoutput == NULL){
	perror("Error: failed to allocate memory for netstat output");
	quit(1);
    }
    
    noecho();
    cbreak();
    
}

void quit(int status){

    if (state.lineoutput != NULL) free(state.lineoutput);

    endwin();

    exit(status);

}

void writeheader(){

    mvaddstr(0, 0, "netmonitor v1.0\n\n");
}


void writelines(){

    int row;

    for (row = state.header.numrows ; row < state.numlines ; row ++){
	mvaddstr(row, 0, state.lineoutput[row]);
	if (row == LINES) break;
    }
}


void display(){

    clear();    
    writeheader();
    writelines();
    refresh();
}

void runnetstat(){
    FILE * pipe;
    char line[MAXCOL];
    int row;
    char * tmp;

    printf("running netstat\n");
    
    pipe = popen("netstat pa", "r");
    
    for (row = 0 ; row < MAXROW ; row ++){
	tmp = fgets(line, MAXCOL, pipe);
	if (tmp == NULL) break;	
	strncpy(state.lineoutput[row], line, COLS);
	state.lineoutput[row][MAXCOL-1] = 0; /* remove newline character */
	                                     /* add null terminator */
    }
    state.numlines = row;
    if (state.numlines == MAXROW)
	perror("Error: netstat output exceeded MAXROW");    

    pclose(pipe);    
}


int main(int argc, char** argv)
{

    initialize();
    
    runnetstat();

    quit(0);

    return 0;
}
