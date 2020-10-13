#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int compare(const void *no1,const void *no2){
    return strcmp(no1,no2);
}

DIR* handleError(){
    DIR *currentDirectory = opendir(".");
    if(currentDirectory==NULL){
        printf("%s\n",strerror(errno));
        exit(-1);
    }
    else{
        struct dirent *cdir = readdir(currentDirectory);
        if(cdir==NULL){
            printf("%s\n",strerror(errno));
            exit(-1);
        }
        else{
            return currentDirectory;
        }
    }
}

void nosort(){
    DIR *currentDirectory =handleError();
    struct dirent *cdir = readdir(currentDirectory);
    while(cdir!=NULL){
        if(strcmp(cdir->d_name,".")!=0 && strcmp(cdir->d_name,"..")!=0){
            printf("%s  ",cdir->d_name);
        }
        cdir = readdir(currentDirectory);
    }
    printf("\n");
}

void sort(){
    DIR *currentDirectory =handleError();
    struct dirent *cdir = readdir(currentDirectory);
    char contents[100][30];
    int count=0;
    while(cdir!=NULL){
        strcpy(contents[count++],cdir->d_name);
        cdir = readdir(currentDirectory);
    }
    char arr[count][30];
    int len=0;
    while (len<count)
    {
        strcpy(arr[len],contents[len]);
        len++;
    }
    qsort(arr, count, 30, compare);
    len=0;
    while(len<count){
        if(strcmp(arr[len],".")!=0 && strcmp(arr[len],"..")!=0){
            printf("%s  ",arr[len]);
        }
        len++;
    }
    printf("\n");
}

void printWithDots(){
    DIR *currentDirectory =handleError();
    struct dirent *cdir = readdir(currentDirectory);
    char contents[100][30];
    int count=0;
    while(cdir!=NULL){
        strcpy(contents[count++],cdir->d_name);
        cdir = readdir(currentDirectory);
    }
    char arr[count][30];
    int len=0;
    while (len<count)
    {
        strcpy(arr[len],contents[len]);
        len++;
    }
    qsort(arr, count, 30, compare);
    len=0;
    while(len<count){
        printf("%s  ",arr[len]);
        len++;
    }
    printf("\n");
}

int main(int argc,char *argv[]){
    if(argv[1]==NULL){
        sort();
    }
    else{
        char *t=strtok(argv[1]," ");
        if(strcmp(t,"-f")==0){
            nosort();
        }
        else if(strcmp(t,"-a")==0 || strcmp(t,"--all")==0){
            printWithDots();
        }
        else if(strcmp(t,"-d")==0 || strcmp(t,"--directory")==0){
            printf(".\n");
        }
    }
    return 0;
}