/* Idea borrowed from Norman Matloff, UC Davis */




#include <curses.h>
#include "netmonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

struct State state;

void quit(int status);
void initialize();
void writeheader();
void writelines();
void runnetstat();
void boldtext();


void initialize(){

    state.scrn = initscr();
    
    state.header.numrows = 5;
    state.currow = state.header.numrows;
    state.curline = 0;
    /*state.lineoutput = malloc(MAXROW * MAXCOL * sizeof(char));
    if (state.lineoutput == NULL){
	perror("Error: failed to allocate memory for netstat output");
	quit(1);
	}*/
    
    noecho();
    cbreak();
    clear();
    keypad(stdscr, TRUE);
}

void quit(int status){

    //if (state.lineoutput != NULL) free(state.lineoutput);

    endwin();

    exit(status);

}


void boldline(){
    attron(A_BOLD);
    mvaddstr(state.currow, 0, state.lineoutput[state.curline]);
    attroff(A_BOLD);
    //refresh();

}

void writeheader(){

    mvaddstr(0, 0, "netmonitor v1.0\n\n");
}


void writelines(){

    int row;
    int linenum;

    for (row = state.header.numrows, linenum = 0 ; linenum < state.numlines ;
	 row ++, linenum ++){
	//printf("%s\n", state.lineoutput[row]);
	mvaddstr(row, 0, state.lineoutput[linenum]);
	//if (row == LINES) break;
    }
}


void display(){

    clear();
    runnetstat();
    writeheader();
    writelines();
    boldline();
    refresh();
}

void runnetstat(){
    FILE * pipe;
    char line[MAXCOL];
    int row;
    char * tmp;

    pipe = popen("netstat -a -p --protocol inet 2>/dev/null", "r");
    
    for (row = 0 ; row < MAXROW ; row ++){
	tmp = fgets(line, MAXCOL, pipe);
	if (tmp == NULL) break;
	strncpy(state.lineoutput[row], line, COLS);
	state.lineoutput[row][MAXCOL-1] = 0; /* remove newline character */
	                                     /* add null terminator */
    }
    state.numlines = row + 1;
    //if (state.numlines == MAXROW)
    //perror("Error: netstat output exceeded MAXROW");    

    pclose(pipe);    
}

void movecursor(int delta){
    int newrow;
    
    newrow = state.currow + delta;
    //if (state.curline == state.numlines - 1 || state.curline == 0 ) return;

    
    if (state.currow == LINES){
	
	//redraw
    }
	
    else if (state.currow == state.header.numrows){
	//redraw
    
    }

    else{
	mvaddstr(state.currow, 0, state.lineoutput[state.curline]);	
	state.curline += delta;
	state.currow += delta;
	boldline();
	
    }
    refresh();
}



void handleinput()
{
    char input;
    //printf("test\n");
    input = getch();
    if (input == 'q') quit(0);
    
    else if (input == KEY_UP) movecursor(1);
    else if (input == 2) movecursor(1);
    else if (input == 3) movecursor(-1);
    else if (input == KEY_DOWN) movecursor(-1);

    printf("input = %i\n", input);



}


int main(int argc, char** argv)
{
    fd_set mask;
    fd_set dummy_mask,temp_mask;
    struct timeval timeout;
    int numfds;

    
    initialize();

    display();
    
    FD_ZERO( &mask );
    FD_ZERO( &dummy_mask );
    FD_SET( STDIN_FILENO, &mask );

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    
    while(1){
	temp_mask = mask;
	numfds = select(FD_SETSIZE, &temp_mask, &dummy_mask, &dummy_mask,
			&timeout);
	if (numfds > 0){
	    handleinput();
	}

	else{
	    //runnetstat();
	    //clear();
	    //refresh();
	    //writeheader();
	    //refresh();
	    display();
	    //refresh();
	    timeout.tv_sec = 2;
	    timeout.tv_usec = 0;

	}
    }
    
    //quit(0);

    return 0;
}
