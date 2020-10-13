#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
int row=1;

void dollar(char buffer[],FILE *file){
    while (fgets(buffer,1024,file)!=NULL)
    {
        if(buffer[0]=='\n'){
            printf("$\n");
        }
        else{
            strtok(buffer, "\n");
            printf("%s$\n",buffer);
        }
    }
}

void show(char buffer[],FILE *file){
    while (fgets(buffer,1024,file)!=NULL)
    {
        printf("%s",buffer);
    }
    printf("\n");
}

void numbered(char buffer[],FILE *file){
    while (fgets(buffer,1024,file)!=NULL)
    {
        printf("  %d%s",row++,buffer);
    }
    printf("\n");
}

FILE *handleErrors(char *fil){
    FILE * file = fopen(fil,"r");
    if(file==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    }
    else return file;
}

int main(int argc,char *argv[]){
    char *t=strtok(argv[1]," ");
    char buffer[1024];
    if(strcmp(t,"-E")==0 || strcmp(t,"--show-ends")==0){
        t=strtok(NULL," ");
        while (t!=NULL)
        {
            FILE * file = handleErrors(t);
            dollar(buffer,file);
            fclose(file);
            t=strtok(NULL," ");
        }
    }
    else if(strcmp(t,"-n")==0 || strcmp(t,"--number")==0){
        t=strtok(NULL," ");
        while (t!=NULL)
        {
            FILE * file = handleErrors(t);
            numbered(buffer,file);
            fclose(file);
            t=strtok(NULL," ");
        }
    }
    else{
        while (t!=NULL)
        {
            FILE * file = handleErrors(t);
            show(buffer,file);
            fclose(file);
            t=strtok(NULL," ");
        }
    }
}