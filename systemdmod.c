#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "netmonitor.h"

int ismanaged(pid_t pid){


    return 1;
}


int restartenabled(pid_t pid){

    return 1;
}

int isactive(pid_t pid){

    
    return 1;
}


int getservicename(pid_t pid, char * name){
    char servicename[80];
    int checkerror;
    char cmdstring[80];
    FILE * pipe;
    char line[MAXCOL];
    char * tmp;
    char * cptr;
    
    snprintf(cmdstring, 80, "systemctl status %u", pid);
    pipe = popen(cmdstring, "r");
    if (pipe == NULL){
	/* handle error */
	return -1;
    }

    /* read first line of output */
    tmp = fgets(line, MAXCOL, pipe);
    if (tmp == NULL) return -1;

    /* point to dot character */
    cptr = strtok(line, " ");
    if (cptr == NULL){
	/* handle error */
	return -1;
    }
    
    /* point to service name */
    cptr = strtok(NULL, " ");
    if (cptr == NULL){
	/* handle error */
	return -1;
    }

    strncpy(name, cptr, 80);        
    
    return 1;
}


int stopservice(pid_t pid){
    char servicename[80];
    int checkerror;
    char cmdstring[80];
    FILE * pipe;

    checkerror = getservicename(pid, servicename);
    if (checkerror < 0){
	/* handle error */
	return -1;
    }

    /* exec systemctl disable SERVICENAME */
    snprintf(cmdstring, 80, "systemctl stop %s", servicename);
    pipe = popen(cmdstring, "r");
    if (pipe == NULL){
	/* handle error */
	return -1;
    }

    checkerror = pclose(pipe);
    if (checkerror != 0){
	/* handle error */
	return -1;
    }
    
    return 1;
}



int disableunit(pid_t pid){
    char servicename[80];
    int checkerror;
    char cmdstring[80];
    FILE * pipe;
    
    checkerror = getservicename(pid, servicename);
    if (checkerror < 0){
	/* handle error */
	return -1;
    }

    /* exec systemctl disable SERVICENAME */
    snprintf(cmdstring, 80, "systemctl disable %s", servicename);
    pipe = popen(cmdstring, "r");
    if (pipe == NULL){
	/* handle error */
	return -1;
    }

    checkerror = pclose(pipe);
    if (checkerror != 0){
	/* handle error */
	return -1;
    }

    return 1;
}
