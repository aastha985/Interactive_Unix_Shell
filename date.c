#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

void gmt(){
    time_t t = time(&t);
    struct tm *tm = gmtime(&t);
    printf("UTC %s", asctime(tm));
}

void fileLastModified(char *filepath){
    struct stat t;
    if(stat(filepath, &t)==-1){
        printf("%s\n",strerror(errno));
        exit(-1);
    }
    else{
        printf("IST %s",ctime(&t.st_mtime));
    }
}

void date(){
    time_t t = time(&t);
    struct tm *tm = localtime(&t);
    if(tm==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    }
    else{
        printf("%s %s",tm->tm_zone,asctime(tm));
    }
}

int main(int argc,char *argv[]){
    if(argv[1]==NULL){
        date();
    }
    else{
        char *t=strtok(argv[1]," ");
        if(strcmp(t,"-r")==0 || strcmp(t,"--reference")==0){
            t=strtok(NULL," ");
            fileLastModified(t);
        }
        else if(strcmp(t,"-u")==0 || strcmp(t,"--utc")==0 || strcmp(t,"--universal")==0){
            gmt();
        }
    }
    return 0;
}
