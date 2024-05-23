#include<stdio.h>
int main(void){
	char* name="Pipe1";
	long f1=fork();
	if(f1<0){
		printf("fork1\n");
		exit(1);
	}
	if(f1==0){
		char* srv="./2srv";
		char* args[]={srv,name,NULL};
		execvp(srv,args);
	}
	//wait();
	long f2=fork();
	if(f2<0){
                printf("fork2\n");
                exit(1);
        }
        if(f2==0){
		char*clt="./2clt";
		char* argc[]={clt,name,NULL};
		execvp(clt,argc);
	}
	waitpid(f1,NULL,0);
	waitpid(f2,NULL,0);
}
