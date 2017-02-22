#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include "netmonitor.h"

char servicename[80];


int ismanaged(pid_t pid){
    int checkerror;
    
    checkerror = getservicename(pid);
    if (checkerror == 0) return 1;
    else return 0;
}


int restartenabled(pid_t pid){

    return 1;
}

int isactive(pid_t pid){

    
    return 1;
}


int getservicename(pid_t pid){
    int checkerror;
    char cmdstring[80];
    FILE * pipe;
    char line[MAXCOL];
    char * tmp;
    char * cptr;

    /* clear name in buffer */
    servicename[0] = '\0';
    
    snprintf(cmdstring, 80, "systemctl status %u", pid);
    pipe = popen(cmdstring, "r");
    if (pipe == NULL){
	/* handle error */
	return -1;
    }

    /* read first line of output */
    tmp = fgets(line, MAXCOL, pipe);
    if (tmp == NULL) return -1;

    checkerror = pclose(pipe);
    if (checkerror == 1){
	/* handle error */
	return ERRSYSDNAME;
    }
    
    
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

    strncpy(servicename, cptr, 80);        

    /* ensure this is a service */
    /* point first to name */
    cptr = strtok(cptr, ".");
    if (cptr == NULL){
	/* handle error */
	return -1;
    }

    /* now point to "service", "scope", or "slice" */
    cptr = strtok(NULL, "\n\0");
    if(strcmp (cptr, "service") != 0) return -1;
    
    return 0;
}


int stopservice(pid_t pid){
    //char servicename[80];
    int checkerror;
    char cmdstring[80];
    FILE * pipe;

    checkerror = getservicename(pid);
    if (checkerror < 0){
	/* handle error */
	return ERRSYSDNAME;
    }

    /* exec systemctl disable SERVICENAME */
    snprintf(cmdstring, 80, "systemctl stop %s", servicename);
    pipe = popen(cmdstring, "r");
    if (pipe == NULL){
	/* handle error */
	return ERRSYSDSTOP;
    }

    checkerror = pclose(pipe);
    if (checkerror != 0){
	/* handle error */
	return -1;
    }
    
    return 0;
}



int disableunit(pid_t pid){
    int checkerror;
    char cmdstring[80];
    FILE * pipe;
    
    checkerror = getservicename(pid);
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
