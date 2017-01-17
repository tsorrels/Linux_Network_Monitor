/* Idea borrowed from Norman Matloff, UC Davis */

#include <sys/types.h>
#include <curses.h>
#include "netmonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <signal.h>

struct State state;
void quit(int status);
void initialize();
void writeheader();
void writelines();
void runnetstat();
void boldtext();
void killprocess();
pid_t parsepid();
void sendkill();
void display();
void clearmessage();


void initialize(){

    state.scrn = initscr();
    
    state.header.numrows = 5;
    state.currow = state.header.numrows;
    state.curline = 0;
    
    noecho();
    cbreak();
    clear();
    keypad(stdscr, TRUE);
}

void quit(int status){
    endwin();
    exit(status);
}


void boldline(){
    attron(A_BOLD);
    mvaddstr(state.currow, 0, state.lineoutput[state.curline]);
    attroff(A_BOLD);
    move(state.currow, 0);
}

void writeheader(){

    mvaddstr(0, 0, "netmonitor v1.0\n\n");
}

void clearmessage(){

    mvaddstr(4, 0, "");
}

void writemessage( char * message){
    mvaddstr(4, 0, message);
    refresh();
}


pid_t parsepid(){
    char tokenline[MAXCOL];
    char pidname[30];
    char pid[10];
    /* char name[20]; */
    char * cptr;
    
    strncpy(tokenline, state.lineoutput[state.curline], MAXCOL);    
    cptr = strtok(tokenline, " \t\n");    
    while (cptr != NULL){
	strncpy(pidname, cptr, 25);
	cptr = strtok(NULL, " \t\n");	
    }

    cptr = strtok(pidname, "/");
    strncpy(pid, cptr, 10);

    if (strcmp(pid, "/") == 0) return (pid_t) 0;

    return (pid_t) atoi(pid);
}

/* checks if state indicates a PID for a process selected for kill; if PID != 0
   then send SIGKILL and reset state.pidkill to 0 */
void sendkill(){
    
    if (state.pidkill != 0) kill(state.pidkill, SIGKILL);
    state.pidkill = 0;
    clearmessage();
    runnetstat();
    display();
}

void killprocess(){    
    pid_t pid;
    char message [100];

    pid = parsepid();

    if (pid == 0){
	writemessage("Cannot kill this process - rerun program as root");
    }

    else{
	snprintf(message, 100, "Kill process PID %u? (Y/n)", pid);
	writemessage(message);
	state.pidkill = pid;
    }
}

void writelines(){

    int row;
    int linenum;
    int startline;

    startline = state.curline - state.currow + state.header.numrows;
    if (startline < 0) startline = 0;
    
    for (row = state.header.numrows, linenum = startline ;
	 linenum < state.numlines ; row ++, linenum ++){
	mvaddstr(row, 0, state.lineoutput[linenum]);
	/* if (row == LINES) break; */
    }

    /* if list shrank, ensure currow is not greater than list size */
    if (state.currow > row) state.currow = row;
    
}


void display(){

    clear();
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
    state.numlines = row;

    pclose(pipe);    
}

void movecursor(int delta){
    int newrow;
    int newline;
    
    newrow = state.currow + delta;
    newline = state.curline + delta;
    if (newline >= state.numlines || newline < 0 ) return;

    
    if (newrow >= LINES){
	state.curline = newline;
	display();
    }
	
    else if (newrow < state.header.numrows){
	state.curline = newline;
	display();    
    }

    else{
	/* first, unbold current line */
	mvaddstr(state.currow, 0, state.lineoutput[state.curline]);	
	state.curline = newline;
	state.currow = newrow;
	boldline();
	refresh();
    }
}



void handleinput()
{
    char input;
    input = getch();
    if (input == 'q') quit(0);
 
    else if (input == 'k') killprocess();
    //else if (input == KEY_UP) movecursor(1);
    else if (input == 2) movecursor(1);
    else if (input == 3) movecursor(-1);
    //else if (input == KEY_DOWN) movecursor(-1);
    else if (input == 'Y' && state.pidkill != 0) sendkill();

}


int main(int argc, char** argv)
{
    fd_set mask;
    fd_set dummy_mask,temp_mask;
    struct timeval timeout;
    int numfds;

    initialize();

    runnetstat();
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
	    runnetstat();
	    display();
	    timeout.tv_sec = 2;
	    timeout.tv_usec = 0;
	}
    }
    
    quit(0);

    /* never reached */
    return 0;
}
