#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void rm(char *t){
    while(t!=NULL){
        if(unlink(t)!=0){
            printf("%s\n",strerror(errno));
            exit(-1);
        }
        t=strtok(NULL," ");
    } 
}

void rmv(char *t){
    while(t!=NULL){
        if(unlink(t)!=0){
            printf("%s\n",strerror(errno));
            exit(-1);
        }
        else{
            printf("Removed '%s'\n",t);
        }
        t=strtok(NULL," ");
    }
}

void rmf(char *t){
    while(t!=NULL){
        unlink(t);
        t=strtok(NULL," ");
    }
}

int main(int argc,char *argv[]){
    char *t=strtok(argv[1]," ");
    if(strcmp(t,"-f")==0 || strcmp(t,"--force")==0){
        t=strtok(NULL," ");
        rmf(t);
    }
    else if(strcmp(t,"-v")==0 || strcmp(t,"--verbose")==0){
        t=strtok(NULL," ");
        rmv(t);
    }
    else{
        rm(t);
    }
    return 0;
}