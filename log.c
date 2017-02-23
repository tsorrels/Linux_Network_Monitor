
#include <stdio.h>
#include "netmonitor.h"
#include <time.h>
#include <string.h>

FILE * filehandle;
time_t rawtime;
struct tm * timeinfo;
char timebuffer[SIZETIMEBUF];

#define SIZETYPE 10

int openlog(){

    filehandle = NULL;
    filehandle = fopen("netmonitor.log", "a");
    if (filehandle == NULL) return -1;

    return 0;
}


int writelog(int type, char* message){
//24
    char logtype[SIZETYPE];
    /* print time, event type, and message */
    switch(type)
    {
    case LOGERROR:
	strncpy(logtype, "error", SIZETYPE);
	break;

    case LOGEVENT:
	strncpy(logtype, "event", SIZETYPE);
	break;

    default:
	strncpy(logtype, "unknown", SIZETYPE);
	break;
    }

    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timebuffer, SIZETIMEBUF, "%D %T", timeinfo);
    fprintf(filehandle, "%s, %s, %s\n", timebuffer, logtype, message);
    
    return 0;
}

int closelog(){

    fclose(filehandle);
    
    return 0;
}
