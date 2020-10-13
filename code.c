#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <errno.h>


int changeDirectoryAndCheckError(char *path){
    int ret = chdir(path);
    if(ret==-1){
        printf("%s\n",strerror(errno));
        return -1;
    }
}

void changeDirectory(char *c){
    if(c==NULL || strcmp("~",c)==0){
        changeDirectoryAndCheckError(getenv("HOME"));
    }
    else{
        c=strtok(c," ");
        if(strcmp(c,"-P")==0){
            char *d = strtok(NULL,"");
            if(d==NULL){
                changeDirectoryAndCheckError(getenv("HOME"));
            }
            else{
                changeDirectoryAndCheckError(d);
            }
        }
        else if(strcmp(c,"--help")==0){
            printf("\ncd: cd[-P--help] [dir]\n\nChange the current directory to dir.\n\nChanges to home directory when no directory is provided or in case of cd ~. Use cd .. to move back one directory.\n\n\tOptions:\n\n\t  -P\t\tAvoids symbolic links. Symlink is resolved before processing instances \"..\".\n\n\t--help\t\tDisplays help for cd and exit status.\n\n\tExit Status:\n\n\t0 when directory is changed successfully.\n\n");
        }
        else{
            changeDirectoryAndCheckError(c);
        }
    }
}

void echo(char *c){
    if(c==NULL){
        printf("\n");
    }
    else if(strlen(c)<2){
        printf("%s\n",c);
    }
    else{
        c=strtok(c," ");
        if(strcmp(c,"-n")==0){
            char * p = strtok(NULL,"");
            if(p==NULL){
                //no newline printed in this case
            }
            else{
                printf("%s",p);
            }
        }
        else if(strcmp(c,"-E")==0){
            printf("%s\n",strtok(NULL,""));
        }
        else{
            char * p = strtok(NULL,"");
            if(p==NULL){
                printf("%s\n",c);
            }
            else{
                printf("%s %s\n",c,p);
            }
        }
    }
}

FILE *handleErrors(char *fil,char *type){
    FILE * file = fopen(fil,type);
    if(file==NULL){
        printf("%s\n",strerror(errno));
        return NULL;
    }
    else return file;
}

int history(char *c,char path[]){
    char historyPath[500];
    strcpy(historyPath,path);
    strcat(historyPath,"/History.txt");
    if(c==NULL){
        FILE *hist = handleErrors(historyPath,"r");
        if(hist==NULL){
            return -1;
        }
        char buffer[1000];
        int row=0;
        while (fgets(buffer,1000,hist))
        {
            row++;
            printf(" %d  %s",row,buffer);
        }
        fclose(hist);
    }
    else{
        c=strtok(c," ");
        if(strcmp(c,"-c")==0){
            FILE *hist = handleErrors(historyPath,"w");
            if(hist==NULL){
                return -1;
            }
            fclose(hist);
        }
        else if(strcmp(c,"-a")==0){
            //handled in main
        }
        else {
            FILE *hist = handleErrors(historyPath,"r");
            if(hist==NULL){
                return -1;
            }
            char buffer[1000];
            int row=0;
            while (fgets(buffer,1000,hist))
            {
                row++;
            }
            fclose(hist);
            int len = row-atoi(c);
            row=0;
            hist = handleErrors(historyPath,"r");
            if(hist==NULL){
                return -1;
            }
            char buf[1000];
            while(fgets(buf,1000,hist)){
                row++;
                if(row>len){
                    printf(" %d  %s",row,buf);
                }
            }
            fclose(hist);
        }
        
    }
}

void addToHistory(char *in,char path[]){
    char historyPath[500];
    strcpy(historyPath,path);
    strcat(historyPath,"/History.txt");
    FILE *hist = fopen(historyPath,"a");
    fputs(in,hist);
    fclose(hist);
}

void external(char *c,char ex[],char path[]){
    char exPath[500];
    strcpy(exPath,path);
    strcat(exPath,ex);
    pid_t pid=fork();
    int status;
    if(pid<0){
        printf("Fork Failed");
    }
    else if(pid==0){
        char *arg[] = {exPath,c,NULL};
        execvp(exPath,arg);
        exit(0);
    }
    else{
        waitpid(pid,&status,0);
    }
}

int checkErrorAndPrintDirectory(){
    char currentWorkingDirectory[500];
    if(getcwd(currentWorkingDirectory,500)!=NULL){
        printf("%s\n",currentWorkingDirectory);
    }
    else{
        printf("%s\n",strerror(errno));
        return -1;
    }
        
}

int pwd(char *c){ 
    if(c==NULL || strlen(c)<2){
        checkErrorAndPrintDirectory();
    }
    else {
        if(strcmp(c,"-P")==0 || strcmp(c,"--physical")==0){
            checkErrorAndPrintDirectory();
        }
        else if(strcmp(c,"--help")==0){
            printf("\npwd: pwd[-P--help]\n\nPrints the current working directory.\n\n\tOptions:\n\n\t  -P\t\tAvoids symbolic links.Shows actual path.\n\n\t--help\t\tDisplays help for pwd and exit status.\n\n\tExit Status:\n\n\t0 except is case of an error.\n\n");
        }
    }
    return 0;
}

int main(){
    system("clear");
    char path[500];
    getcwd(path,500);
    while(true){
        size_t len = 1024;
        char *in;
        in = (char *) malloc (len);
        printf("\033[1;36m");
        printf("aastha@unix_shell: ");
        printf("\033[01;33m");
        printf("$ ");
        printf("\033[0m");
        getline (&in, &len, stdin);
        if(strcmp(in,"\n")==0){
            continue;
        }
        addToHistory(in,path);
        in = strtok(in,"\n");
        char * command=strtok(in," ");
        char *c = strtok(NULL,"");
        if(strcmp(command,"exit")==0){
            if(c==NULL){
                exit(0);
            }
            else if(strcmp(c,"--help")==0){
                printf("\nexit: exit [n]\n\tExit the shell.\n\n\tExits the shell with status of n.\n\tExits with status of last executed command in case when n is not provided.\n\n");
            }
            else{
                exit(atoi(c));
            }
        }
        else if(strcmp(command,"history")==0){
            history(c,path);
        }
        else if(strcmp(command,"echo")==0){
            echo(c);
        }
        else if(strcmp(command,"pwd")==0){
            pwd(c);
        }
        else if(strcmp(command,"cd")==0){
            changeDirectory(c);
        }
        else if(strcmp(command,"ls")==0){
            external(c,"/ls",path);
        }
        else if(strcmp(command,"cat")==0){
            external(c,"/cat",path);
        }
        else if(strcmp(command,"date")==0){
            external(c,"/date",path);
        }
        else if(strcmp(command,"rm")==0){
            external(c,"/remove",path);
        }
        else if(strcmp(command,"mkdir")==0){
            external(c,"/mkdir",path);
        }
        else{
            printf("Command Not Found\n");
        }
    }
    return 0;
}