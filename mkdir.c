#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

void mk(char *t){
    while(t!=NULL){
        if(mkdir(t,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0){
            printf("%s\n",strerror(errno));
            exit(-1);
        }
        t=strtok(NULL," ");
    } 
}

void mkv(char *t){
    while(t!=NULL){
        if(mkdir(t,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0){
            printf("%s\n",strerror(errno));
            exit(-1);
        }
        else{
            printf("mkdir: created directory '%s'\n",t);
        }
        t=strtok(NULL," ");
    }
}

void mkp(char t[]){
    char s[100]="";
    for(int i=0;i<strlen(t);i++){
        if(t[i]=='/'){
            if(mkdir(s,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0){
                printf("%s\n",strerror(errno));
                exit(-1);
            }
        }
        else if(i==strlen(t)-1){
            strncat(s, &t[i], 1);
            if(mkdir(s,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0){
                printf("%s\n",strerror(errno));
                exit(-1);
            }
        }
        strncat(s, &t[i], 1);
    } 
}

int main(int argc,char *argv[]){
    char *t=strtok(argv[1]," ");
    if(strcmp(t,"-p")==0 || strcmp(t,"--parents")==0){
        t=strtok(NULL," ");
        mkp(t);
    }
    else if(strcmp(t,"-v")==0 || strcmp(t,"--verbose")==0){
        t=strtok(NULL," ");
        mkv(t);
    }
    else{
        mk(t);
    }
    return 0;
}