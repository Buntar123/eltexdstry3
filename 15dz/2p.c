#include<stdio.h>
#include<signal.h>
#include<unistd.h>
int main(void){
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGINT);
	if(sigprocmask(SIG_BLOCK,&mask,NULL)==-1){
		printf("sigprocmask_error\n");
	}
	while(1){pause();}
}
