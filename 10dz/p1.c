#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(void){
	int f_pid=fork();
	if(f_pid<0){printf("fork\n");exit(1);}
	if(f_pid>0){
		printf("Parent: pid - %d ppid - %d\n",getpid(),getppid());
		pid_t w_pid = wait();
		printf("Exit child pid - %d\n",w_pid);
	}
	else if(f_pid==0){
		printf("Child: pid - %d ppid - %d\n",getpid(),getppid());
		exit(1);
	}
}
