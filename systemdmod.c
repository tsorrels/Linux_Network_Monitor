




int ismanaged(pid_t pid){


    return 1;
}



int isactive(pid_t pid){

    
    return 1;
}

int getservicename(char * name){


    return 1;
}


int disableunit(pid_t pid){
    char servicename[80];
    int checkerror;
    char cmdstring[80];
    
    checkerror = getservicename(servicename);
    if (checkerror < 0){
	/* handle error */
	return -1
    }

    /* exec systemctl disable SERVICENAME */

    //snprintf(smdstring, 80, "sys
    

    return 1;
}
