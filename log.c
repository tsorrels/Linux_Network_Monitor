
#include <stdio.h>
#include "netmonitor.h"
#include <time.h>

FILE * filehandle;
time_t rawtime;
struct tm * timeinfo;
char timebuffer[SIZETIMEBUF];

int openlog(){

    filehandle = NULL;
    filehandle = fopen("netmonitor.log", "a");
    if (filehandle == NULL) return -1;

    return 0;
}


int writelog(int type, char* message){
//24
    /* print time, event type, and message */
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timebuffer, SIZETIMEBUF, "%D %T", timeinfo);
    fprintf(filehandle, "%s, %s\n", timebuffer, message);
    
    return 0;
}

int closelog(){

    fclose(filehandle);
    
    return 0;
}
