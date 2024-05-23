#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
void first_command(char** result,int* fd){
        if(pipe(fd)==-1){
                printf("Pipe\n");
                exit(1);
        }
        pid_t f1=fork();
        if(f1<0){
                printf("Fork\n");
                exit(1);
        }
        if(f1==0){
                close(fd[0]);
                dup2(fd[1],1);
                execvp(result[0],result);
        }
        free(result);
        waitpid(f1,NULL,0);

}
void last_command(char** result,int* fd){
        pid_t f1=fork();
        if(f1<0){
                printf("Fork\n");
                exit(1);
        }
        if(f1==0){
                close(fd[1]);
                dup2(fd[0],0);
                execvp(result[0],result);
        }
        free(result);
        close(fd[0]);
        close(fd[1]);
        waitpid(f1,NULL,0);
}
void middle_command(char** result,int*fd_old,int* fd_new){
        if(pipe(fd_new)==-1){
                printf("Pipe\n");
                exit(1);
        }
        pid_t f1=fork();
        if(f1<0){
                printf("Fork\n");
                exit(1);
        }
        if(f1==0){
                close(fd_old[1]);
                dup2(fd_old[0],0);
                close(fd_new[0]);
                dup2(fd_new[1],1);
                execvp(result[0],result);
        }
        free(result);
        close(fd_old[0]);
        close(fd_old[1]);
        waitpid(f1,NULL,0);
}
void entire_command(char** args){
        long f1=fork();
        if(f1<0){printf("err_fork\n");}
        if(f1==0){
                execvp(args[0],args);
        }
        waitpid(f1,NULL,0);
}

